#pragma once

#include "dto/DTOs.hpp"
#include "db/UserDB.hpp"

#include <print>
#include <memory>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>
#include <oatpp-swagger/Generator.hpp>

#include "bcrypt/BCrypt.hpp"



#include OATPP_CODEGEN_BEGIN(ApiController)
class UserController: public oatpp::web::server::api::ApiController {
public:
    UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectmapper),
                   OATPP_COMPONENT(std::shared_ptr<UserDb>, database))
    : oatpp::web::server::api::ApiController(objectmapper),
      m_Database(database){}

public:
    ENDPOINT("POST","/signup", signup,
        BODY_DTO(Object<UserDTO>, userDTO)) {
        auto name = userDTO->name;
        auto password = userDTO->password;

        std::println("name: {}", name->c_str());
        std::println("password before hashing: {}", password->c_str());

        std::string hash = BCrypt::generateHash(password->c_str());
        std::println("password after hashing: {}", hash);

        // Execute the database query
        auto dbResult = m_Database->createUser(name, hash);

        // Check if it failed (e.g., username already exists)
        if(!dbResult->isSuccess()) {
            OATPP_LOGe("UserController", "Database error: %s", dbResult->getErrorMessage()->c_str());
            return createResponse(Status::CODE_500, "Failed to create user in database.");
        }

        // TODO: Return the success response
        return createResponse(Status::CODE_201, "User Created Successfully");
    }

    ENDPOINT("POST","/login", login,
        BODY_DTO(Object<UserDTO>, userDTO)) {
        auto username = userDTO->name;
        auto password = userDTO->password;

        auto dbresult = m_Database->getUserByUsername(username);
        if(!dbresult->isSuccess()) {
            return createResponse(Status::CODE_500, "Database error");
        }

        auto fetcher = dbresult->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
        if(fetcher->size() == 0) {
            return createResponse(Status::CODE_404, "User not found");
        }

        auto storedHash = fetcher[0]->password;
        if(BCrypt::validatePassword(userDTO->password->c_str(), storedHash->c_str())) {
            return createResponse(Status::CODE_200, "Login successful");
        } else {
            return createResponse(Status::CODE_401, "Invalid password");
        }
    }

    ENDPOINT_INFO(root) {
        info->summary = "hello";
        info->addResponse<Object<MessageDTO>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/hello", root) {
        auto dto = MessageDTO::createShared();
        dto->statusCode = 200;
        dto->message = "အမျိုးဘာသာသနာ";
        return createDtoResponse(Status::CODE_200, dto);
    }

private:
    std::shared_ptr<UserDb> m_Database;
};
#include OATPP_CODEGEN_END(ApiController)
