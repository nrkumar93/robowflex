#include <robowflex_library/macros.h>
#include <robowflex_library/log.h>
#include <robowflex_library/io.h>
#include <robowflex_library/planning.h>
#include <robowflex_library/robot.h>
#include <robowflex_library/scene.h>

#include <robowflex_search/search_interface.h>

using namespace robowflex;

search::SearchInterfacePlanner::SearchInterfacePlanner(const RobotPtr &robot, const std::string &name)
  : Planner(robot, name)
{
}

bool search::SearchInterfacePlanner::initialize(const std::string& ns, const std::string& gpName, const std::string &config_file, const search::Settings settings)
{
    if (!loadSearchConfig(handler_, config_file, configs_))
        return false;

    settings.setParam(handler_);

    const auto robot = getRobot();
    moveit::core::RobotModelConstPtr model = robot->getModel();

    context_ = std::make_shared<ims_interface::IMSPlanningContext>("ims_planning_context", ns, gpName, model);

    return true;
}


void search::SearchInterfacePlanner::preRun(const SceneConstPtr &scene,
                                        const planning_interface::MotionPlanRequest &request)
{
    refreshContext(scene, request, true);
}

void printJointPositions(const std::vector<double>& positions, const std::string& state_name)
{
    std::cout << state_name << " State Joint Positions:" << std::endl;
    for (size_t i = 0; i < positions.size(); ++i)
    {
        std::cout << "  Joint " << i << ": " << positions[i] << std::endl;
    }
}

planning_interface::MotionPlanResponse search::SearchInterfacePlanner::plan(
    const SceneConstPtr &scene, const planning_interface::MotionPlanRequest &request)
{
    planning_interface::MotionPlanResponse response;
    response.error_code_.val = moveit_msgs::MoveItErrorCodes::SUCCESS;

    refreshContext(scene, request);

    if (pre_plan_callback_)
        pre_plan_callback_(context_, scene, request);


    context_->solve(response);
    return response;
}

std::map<std::string, Planner::ProgressProperty> search::SearchInterfacePlanner::getProgressProperties(
    const SceneConstPtr &scene, const planning_interface::MotionPlanRequest &request) const
{
    refreshContext(scene, request);
    return {};
}

void search::SearchInterfacePlanner::refreshContext(const SceneConstPtr &scene,
                                                const planning_interface::MotionPlanRequest &request,
                                                bool force) const
{
    const auto &scene_id = scene->getKey();
    const auto &request_hash = IO::getMessageMD5(request);

    bool same_scene = compareIDs(scene_id, last_scene_id_);
    bool same_request = request_hash == last_request_hash_;

    // if (not force and same_scene and same_request)
    // {
    //     RBX_INFO("Reusing Cached Context!");
    //     return;
    // }

    last_scene_id_ = scene_id;
    last_request_hash_ = request_hash;
    context_->setPlanningScene(scene->getSceneConst());
    context_->setMotionPlanRequest(request);

    RBX_INFO("Refreshed Context!");
}

std::vector<std::string> search::SearchInterfacePlanner::getPlannerConfigs() const
{
    return configs_;
}

// ims_interface::IMSInterface &search::SearchInterfacePlanner::getInterface() const
// {
//     if (!interface_)
//     {
//         RBX_WARN("Interface is not initialized before call to SearchInterfacePlanner::initialize.");
//     }
//     return *interface_;
// }

void search::SearchInterfacePlanner::setPrePlanCallback(const PrePlanCallback &prePlanCallback)
{
    pre_plan_callback_ = prePlanCallback;
}
