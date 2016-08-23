/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#include "betascheduler_gausskronrod.h"
#include <stdexcept>
#include "utilities.h"
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include "chainprinter.h"

namespace aims
{
    void BetaScheduler_GaussKronrod::Print(ChainServices &services)
    {
        services.chain_printer->Print(*this);
    }


    BetaScheduler_GaussKronrod::BetaScheduler_GaussKronrod(double gamma): BetaScheduler(gamma) 
    {
        // Note that the node points and weights have been adjusted tobeta_candidate
        //  convert the standard integral over [-1,1] to the required integral
        //  interval over [0, 1]
        
        // Node points and weights
        // Information for 7-point Gauss rule
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.949107912342759), 0.5 * 0.129484966168870));
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.741531185599394), 0.5 * 0.279705391489277));
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.405845151377397), 0.5 * 0.381830050505119));
        this->gauss_data_.push_back(std::pair<double, double> (0.5, 0.5 * 0.417959183673469));
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.405845151377397), 0.5 * 0.381830050505119));
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.741531185599394), 0.5 * 0.279705391489277));
        this->gauss_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.949107912342759), 0.5 * 0.129484966168870));
        
        // Information for 15-point Kronrod rule
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.991455371120813), 0.5 * 0.022935322010529));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.949107912342759), 0.5 * 0.063092092629979));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.864864423359769), 0.5 * 0.104790010322250));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.741531185599394), 0.5 * 0.140653259715525));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.586087235467691), 0.5 * 0.169004726639267));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.405845151377397), 0.5 * 0.190350578064785));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 - 0.207784955007898), 0.5 * 0.204432940075298));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5, 0.5 * 0.209482141084728));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.207784955007898), 0.5 * 0.204432940075298));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.405845151377397), 0.5 * 0.190350578064785));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.586087235467691), 0.5 * 0.169004726639267));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.741531185599394), 0.5 * 0.140653259715525));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.864864423359769), 0.5 * 0.104790010322250));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.949107912342759), 0.5 * 0.063092092629979));
        this->kronrod_data_.push_back(std::pair<double, double> (0.5 * (1.0 + 0.991455371120813), 0.5 * 0.022935322010529));

        this->integral_data_["trapezoid"] = 0.0;
        this->integral_data_["gauss"] = 0.0;
        this->integral_data_["kronrod"] = 0.0;

        this->last_gk_point_ = 0;
        this->done_ = false;
        this->triggered_ = false;
    }

    #ifdef DEBUG
    void BetaScheduler_GaussKronrod::PrintBetaData()
    				{ for (std::vector<BetaData>::iterator it = this->beta_data_.begin(); it < this->beta_data_.end(); it++) 
					std::cout << it->j << "\t" << it->beta << "\t" << it->is_gauss_point << "\t" << it->is_kronrod_point << 
					"\t" << it->gauss_point_number << "\t" << it->kronrod_point_number << "\t" << it->mean_log_likelihood << std::endl;}
    #endif

    void BetaScheduler_GaussKronrod::Update(ChainData const &data_previous, ChainServices const &services)
    {
        BetaData value;
        value.j = data_previous.j;
        value.beta = data_previous.beta;
        value.is_kronrod_point = this->triggered_;
        value.is_gauss_point = (this->triggered_ && this->last_gk_point_ % 2 == 0 && this->last_gk_point_ != 0);
        value.mean_log_likelihood = utilities::SimpleMoment(data_previous.log_likelihood, 1);

        if(!this->triggered_)
        {
            value.gauss_point_number = -1;
            value.kronrod_point_number = -1;
        }
        else
        {
            if(this->last_gk_point_ % 2 == 0 && this->last_gk_point_ != 0)
            {
                uint64_t gauss_point_number = (this->last_gk_point_ - 1u) / 2u;
                value.gauss_point_number = gauss_point_number;
            }
            else value.gauss_point_number = -1;

            value.kronrod_point_number = this->last_gk_point_ - 1;
        }

        beta_data_.push_back(value);
        if(this->done_) this->Integrate();
    }

    double BetaScheduler_GaussKronrod::Solve(ChainData const &data_previous, ChainData const &data_current, ChainServices const &services)
    {
        const double constant = -(log(this->gamma_) + log(data_previous.num_samples));
        double beta_candidate = utilities::BetaBinarySearch(data_previous, constant);
        double beta_final;

        if(this->last_gk_point_ == this->kronrod_data_.size())
        {
            if(data_previous.num_samples_effective > this->gamma_ * data_previous.num_samples ||
                beta_candidate >= 1.0)
            {
                this->done_ = true;
                this->triggered_ = false;
                beta_final = 1.0;
            }
            else 
            {
                this->triggered_ = false;
                beta_final = beta_candidate;
            }
        }
        else
        {
            double beta_other_candidate = this->kronrod_data_[this->last_gk_point_].first; 
            if(beta_candidate > beta_other_candidate)
            {
                this->last_gk_point_++;
                this->triggered_ = true;
                beta_final = beta_other_candidate;
            }
            else 
            {
                this->triggered_ = false;
                beta_final = beta_candidate;
            }
        }

        if(std::abs(beta_final) == std::numeric_limits<double>::infinity() ||
            beta_final < 0.0 || std::isnan(beta_final) || beta_final > 1.0)
        {
            std::ostringstream os;
            os << "BetaScheduler_GaussKronrod::Solve generated an unexpected value. ";
            os << "beta_candidate = " << beta_candidate << ". ";
            throw std::logic_error(os.str());
        }

        return beta_final;
    }

    void BetaScheduler_GaussKronrod::Integrate()
    {
        if(!this->done_)
            throw std::logic_error("BetaSchedulerGaussKronrod::Integrate called before AIMS completion");

        #ifdef DEBUG_1
            uint64_t count_g = 0, count_k = 0;
        #endif
        
        for(uint64_t i = 0; i < this->beta_data_.size() - 1; i++)
        {
            // Trapezoidal integral
            this->integral_data_["trapezoid"] += 0.5 * (this->beta_data_[i + 1].beta -this->beta_data_[i].beta) * 
                (this->beta_data_[i].mean_log_likelihood + this->beta_data_[i + 1].mean_log_likelihood);
            // Gauss rule
            if(this->beta_data_[i].is_gauss_point)
            {
                #ifdef DEBUG_1
                    count_g++;
                #endif
                double weight =  this->gauss_data_[this->beta_data_[i].gauss_point_number].second;
                double mean_log_likelihood = this->beta_data_[i].mean_log_likelihood;
                this->integral_data_["gauss"] += weight * mean_log_likelihood;
            }
            // Kronrod rule
            if(this->beta_data_[i].is_kronrod_point)
            {
                #ifdef DEBUG_1
                    count_k++;
                #endif
                double weight = this->kronrod_data_[this->beta_data_[i].kronrod_point_number].second;
                double mean_log_likelihood = this->beta_data_[i].mean_log_likelihood;
                this->integral_data_["kronrod"] += weight * mean_log_likelihood;
            }
        }

        #ifdef DEBUG_1
            std::cout << "counts: g, k = " << count_g << ", " << count_k << std::endl;
        #endif

    }

    std::map<std::string, double> BetaScheduler_GaussKronrod::IntegralData()
    {
        if(!this->done_)
            throw std::logic_error("BetaScheduler::IntegralData() called prematurely. (done_ is false).");
        return this->integral_data_;
    }

    bool BetaScheduler_GaussKronrod::Done()
    {
        return this->done_;
    }
    
}