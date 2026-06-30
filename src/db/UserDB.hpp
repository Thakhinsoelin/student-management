#pragma once

#include <oatpp-postgresql/orm.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/orm/QueryResult.hpp>
#include <oatpp/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient) // <-- Activates Oat++ ORM magic

class UserDb : public oatpp::orm::DbClient {
public:
    UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
        : oatpp::orm::DbClient(executor)
    {
        // Automatically create a simple table if it doesn't exist yet
        executeQuery("CREATE TABLE IF NOT EXISTS students (id SERIAL PRIMARY KEY, username TEXT, password TEXT);", {});
    }

    // A sample query mapping a C++ function to a SQL execution statement
    QUERY(createUser, 
          "INSERT INTO public.users (username, password) VALUES (:username, :password);", 
          PARAM(oatpp::String, username), 
          PARAM(oatpp::String, password));
    
    QUERY(getUserByUsername, 
      "SELECT password FROM public.users WHERE username = :username;", 
      PARAM(oatpp::String, username));
};
#include OATPP_CODEGEN_END(DbClient)
