#include <moveit/ompl_interface/model_based_planning_context.h>

#include <robowflex_library/io/handler.h>
#include <robowflex_library/robot.h>
#include <robowflex_library/scene.h>
#include <robowflex_library/planning.h>

#include <robowflex_ompl/ompl.h>

using namespace robowflex;

OMPL::OMPLInterfacePlanner::OMPLInterfacePlanner(const RobotPtr &robot, const std::string &name)
  : Planner(robot, name), interface_(robot->getModel(), handler_.getHandle())
{
}

bool OMPL::OMPLInterfacePlanner::initialize(const std::string &config_file, const OMPL::Settings settings)
{
    if (!loadOMPLConfig(handler_, config_file, configs_))
        return false;

    settings.setParam(handler_);

    interface_.simplifySolutions(settings.simplify_solutions);

    auto &pcm = interface_.getPlanningContextManager();
    pcm.setMaximumSolutionSegmentLength(settings.maximum_waypoint_distance);
    pcm.setMinimumWaypointCount(settings.minimum_waypoint_count);

    pcm.setMaximumPlanningThreads(settings.max_planning_threads);
    pcm.setMaximumGoalSamples(settings.max_goal_samples);
    pcm.setMaximumStateSamplingAttempts(settings.max_state_sampling_attempts);
    pcm.setMaximumGoalSamplingAttempts(settings.max_goal_sampling_attempts);

    if (settings.max_solution_segment_length > std::numeric_limits<double>::epsilon())
        pcm.setMaximumSolutionSegmentLength(settings.max_solution_segment_length);

    return true;
}

planning_interface::MotionPlanResponse OMPL::OMPLInterfacePlanner::plan(
    const SceneConstPtr &scene, const planning_interface::MotionPlanRequest &request)
{
    planning_interface::MotionPlanResponse response;
    response.error_code_.val = moveit_msgs::MoveItErrorCodes::FAILURE;

    ompl_interface::ModelBasedPlanningContextPtr context =
        interface_.getPlanningContext(scene->getSceneConst(), request);

    if (!context)
        return response;

    context->clear();
    bool result = context->solve(response);

    return response;
}

const std::vector<std::string> OMPL::OMPLInterfacePlanner::getPlannerConfigs() const
{
    return configs_;
}
