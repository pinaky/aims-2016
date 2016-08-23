/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#ifdef DEBUG
    #include "debug.h"
#endif

#include <memory>
#include <string>
#include <random>

#include "chain.h"
#include "numsamplesschedulerbuilder_constant.h"
#include "acceptanceprobabilityservicebuilder_regular.h"
#include "betaschedulerbuilder_gausskronrod.h"
#include "proposalschedulerbuilder_constantrwmhisotropicnormal.h"
#include "priorbuilder_generaluniform.h"
#include "forwardmodelbuilder_gaussian.h"
#include "chainprinterbuilder_json.h"
#include "chainservicesbuilder.h"

#ifdef _OPENMP
    #include <omp.h>
#endif

int main()
{
    #ifdef _OPENMP
        uint64_t num_max_threads = omp_get_max_threads();
        if(num_max_threads != 1) omp_set_num_threads(num_max_threads - 1);
    #endif

    std::string s;
    std::cout << "*** AIMS input data ***" << std::endl;

    // Generate a random engine
    std::random_device rd;
    std::unique_ptr<std::mt19937_64> random_engine(new std::mt19937_64(rd()));

    // To determine the number of samples required at each AIMS stage
    std::unique_ptr<aims::NumSamplesScheduler> num_samples_scheduler(aims::NumSamplesSchedulerBuilder_Constant().BuildFromInput());

    // To create the acceptance probability service that calculates acceptance probabilities during chain construction
    std::unique_ptr<aims::AcceptanceProbabilityService> acceptance_probability_service(aims::AcceptanceProbabilityServiceBuilder_Regular().Build());

    // To determine how beta is updated at each AIMS stage
    std::unique_ptr<aims::BetaScheduler> beta_scheduler(aims::BetaSchedulerBuilder_GaussKronrod().BuildFromInput());

    // To determine how the AIMS local proposal is updated at each stage 
    std::unique_ptr<aims::ProposalScheduler> proposal_scheduler(aims::ProposalSchedulerBuilder_ConstantRWMHIsotropicNormal().BuildFromInput());

    // A prior distribution for initialization
    std::unique_ptr<aims::Prior> prior_instance(aims::PriorBuilder_GeneralUniform().BuildFromInput());

    // The forward model for the problem
    std::unique_ptr<aims::ForwardModel> forward_model(aims::ForwardModelBuilder_Gaussian().BuildFromInput());

    // To print an AIMS chain
    std::unique_ptr<aims::ChainPrinter> chain_printer(aims::ChainPrinterBuilder_JSON().BuildFromInput());

    #ifdef DEBUG
    if(!(chain_printer.get())) std::cout << "chain_printer is null to begin with." << std::endl;
    #endif

    std::unique_ptr<aims::ChainServices> chain_services(aims::ChainServicesBuilder()
        .SetPrior(std::move(prior_instance))
        .SetNumSamplesScheduler(std::move(num_samples_scheduler))
        .SetBetaScheduler(std::move(beta_scheduler))
        .SetProposalScheduler(std::move(proposal_scheduler))
        .SetAcceptanceProbabilityService(std::move(acceptance_probability_service))
        .SetForwardModel(std::move(forward_model))
        .SetRandomEngine(std::move(random_engine))
        .SetChainPrinter(std::move(chain_printer))
        .Build()   
    );

    // Create the initial chain; the one with prior samples
    std::unique_ptr<aims::Chain> current(new aims::Chain(*chain_services));
        /*aims::ChainBuilder_Initial()
        .SetPrior(std::move(prior_instance))
        .SetNumSamplesScheduler(std::move(num_samples_scheduler))
        .SetBetaScheduler(std::move(beta_scheduler))
        .SetProposalScheduler(std::move(proposal_scheduler))
        .SetAcceptanceProbabilityService(std::move(acceptance_probability_service))
        .SetForwardModel(std::move(forward_model))
        .SetRandomEngine(std::move(random_engine))
        .SetChainPrinter(std::move(chain_printer))
        .Build());
*/
    bool done = false;
    // Run the AIMS process
    while(!done)
    {
         // Create the next chain from the current
        std::unique_ptr<aims::Chain> next(new aims::Chain(*chain_services, *current));
        next->Print(*chain_services, *current);
        done = next->Done();
        // Move current to the next (destroying the current)
        current = std::move(next);
    }

    current->Print(*chain_services, *current);
    chain_services->beta_scheduler->Print(*chain_services);
    return 0;
}
