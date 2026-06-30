#include "AppComponent.hpp"
#include "controller/UserController.hpp"
#include <oatpp/data/mapping/ObjectMapper.hpp>
#include <oatpp/macro/component.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp-swagger/ControllerPaths.hpp>

#include <print>
#include <memory>
#include <stdexcept>

void run() {
    try{ 
        AppComponent components{};
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

        auto myController = std::make_shared<UserController>();

        router->addController(myController);

        auto swaggerController = oatpp::swagger::Controller::createShared(myController->getEndpoints());
        router->addController(swaggerController);
        

        /* Create HTTP connection handler with router */
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

        /* Create TCP connection provider */

        oatpp::network::Server server(connectionProvider, connectionHandler);

        /* Priny info about server port */
        OATPP_LOGi("MyApp", "Server running on port {}", connectionProvider->getProperty("port").getData());
        /* Create server which takes provided TCP connections and passes them to HTTP connection handler */

        /* Print info about server port */
        //std::println("MyApp", "Server is running on port {}", connectionProvider->getProperty("port").getData());

        /* Run server */
        server.run(); 
    } catch(const std::runtime_error& e) {
        std::println("Server crash {}", e.what());
    } catch(...) {
        std::println("unknown exception caught");
    }
}
int main() {
    /* Init oatpp Environment */
    oatpp::Environment::init();


    /* Run App */
    run();

    /* Destroy oatpp Environment */
    oatpp::Environment::destroy();

    return 0;
}
