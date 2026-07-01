#pragma once
#include <oatpp/macro/component.hpp>
#include <oatpp/web/server/HttpRouter.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)

class MessageDTO : public oatpp::DTO {
    DTO_INIT(MessageDTO, DTO)

    DTO_FIELD(Int32, statusCode);
    DTO_FIELD_INFO(statusCode) { info->required = true; }
    DTO_FIELD(String, message);
    DTO_FIELD_INFO(message) { info->required = true; }
};
#include OATPP_CODEGEN_END(DTO)


#include OATPP_CODEGEN_BEGIN(DTO)

class StudentDTO : public oatpp::DTO {
    DTO_INIT(StudentDTO, DTO)

    DTO_FIELD(String, name);
    DTO_FIELD(Int32, studentID);
    DTO_FIELD(String, fatherName);
    DTO_FIELD(String, motherName);
    DTO_FIELD(String, address);
    DTO_FIELD(String, phoneNum);
    DTO_FIELD(String, email);

};
#include OATPP_CODEGEN_END(DTO)

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDTO: public oatpp::DTO {
    DTO_INIT(UserDTO, DTO)

    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->required = true; }
    DTO_FIELD(String, password);
    DTO_FIELD_INFO(password) { info->required = true; }

};
#include OATPP_CODEGEN_END(DTO)


