#pragma once

#include <oatpp/web/server/HttpRouter.hpp>
#include <memory>
#include <oatpp/Types.hpp>
#include <oatpp/json/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/macro/component.hpp>

#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp-postgresql/ConnectionProvider.hpp>
#include <oatpp-postgresql/orm.hpp>
#include "db/UserDB.hpp"

class AppComponent {
public:

    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::stoi(portEnv) : 9000; // Default to 9000 if not set
    //
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)(
        oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", (unsigned short)port, oatpp::network::Address::IP_4})
    );

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(
        oatpp::web::server::HttpRouter::createShared()
    );

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        return std::make_shared<oatpp::json::ObjectMapper>();
    }());


    // --- Database Components ---

    // 1. Create the Connection Provider using your Environment Variable
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionProvider>, dbConnectionProvider)([] {
        const char* dbUrlEnv = std::getenv("DBURL");
        if (!dbUrlEnv) {
            OATPP_LOGe("AppComponent", "FATAL: DBURL environment variable is not set!");
            throw std::runtime_error("DBURL not set");
        }
        return std::make_shared<oatpp::postgresql::ConnectionProvider>(dbUrlEnv);
    }());

    // 2. Create the Database Executor (handles the actual threading/execution of queries)
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, dbExecutor)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionProvider>, connectionProvider);
        return std::make_shared<oatpp::postgresql::Executor>(connectionProvider);
    }());

    // 3. Create your UserDb Client, injecting the Executor into it
    OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, dbExecutor);
        return std::make_shared<UserDb>(dbExecutor);
    }());

    // --- Swagger Components ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
        oatpp::swagger::DocumentInfo::Builder builder;
        builder.setTitle("Student Management System")
               .setDescription("API Documentation")
               .setVersion("1.0")
               .addServer("http://localhost:9000", "server");
        return builder.build();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        // OATPP_SWAGGER_RES_PATH is defined by your build system (usually via CMake)
        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());

    
private:
};
