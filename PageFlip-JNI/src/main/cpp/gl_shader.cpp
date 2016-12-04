/*
 * Copyright (C) 2016 eschao <esc.chao@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gl_shader.h"
#include "constant.h"
#include "error.h"

GLShader::GLShader()
        : m_shader_ref(Constant::kGlInvalidRef)

{
}

GLShader::~GLShader()
{
    clean();
}

int GLShader::load(GLenum type, const char *shader_glsl)
{
    clean();

    m_shader_ref= glCreateShader(type);
    if (m_shader_ref == Constant::kGlInvalidRef) {
        return Error::ERR_GL_CREATE_SHADER_REF;
    }

    glShaderSource(m_shader_ref, 1, &shader_glsl, NULL);
    glCompileShader(m_shader_ref);

    GLint compiled = 0;
    glGetShaderiv(m_shader_ref, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint info_len = 0;
        glGetShaderiv(m_shader_ref, GL_INFO_LOG_LENGTH, &info_len);

        if (info_len) {
            info_len = check_err_desc_len(info_len);
            glGetShaderInfoLog(m_shader_ref, info_len, NULL, err_desc);
            glDeleteShader(m_shader_ref);
            m_shader_ref = Constant::kGlInvalidRef;
        }

        return Error::ERR_GL_COMPILE_SHADER;
    }

    return Error::OK;
}

void GLShader::clean()
{
    if (m_shader_ref != Constant::kGlInvalidRef) {
        glDeleteShader(m_shader_ref);
        m_shader_ref = Constant::kGlInvalidRef;
    }
}