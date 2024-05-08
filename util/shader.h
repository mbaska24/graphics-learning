#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <../include/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

using namespace std;

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;


		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vstream, fstream;
			vstream << vShaderFile.rdbuf();
			fstream << fShaderFile.rdbuf();
			
			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vstream.str();
			fragmentCode = fstream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		cout << vShaderCode << endl;
		int success;
		char infoLog[512];
		unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v_shader, 1, &vShaderCode, NULL);
		glCompileShader(v_shader);

		glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(v_shader, 512, NULL, infoLog);
			cout << "VERTEX_SHADER_COMPILE: " << infoLog << "\n" << endl;
		}

		unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f_shader, 1, &fShaderCode, NULL);
		glCompileShader(f_shader);

		glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(f_shader, 512, NULL, infoLog);
			cout << "FRAGMENT_SHADER_COMPILE: " << infoLog << "\n" << endl;
		}

		this->ID = glCreateProgram();

		glAttachShader(this->ID, v_shader);
		glAttachShader(this->ID, f_shader);

		glLinkProgram(this->ID);

		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		}

		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
	}

	void use() {
		glUseProgram(this->ID);
	}

	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
	}

	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void setColor(const string& name, float r, float g, float b, float a) {
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), r, g, b, a);
	}

	void setMatrix(const string& name, float* matrix) {
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, matrix);
	}

	void kill() {
		glDeleteProgram(this->ID);
	}

};


#endif


