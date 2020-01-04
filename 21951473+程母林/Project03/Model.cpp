#include "Model.h"

Model::Model() {

}

Model::Model(const char *filepath) {
	LoadModel(filepath);
}

void Model::LoadModel(const char *filepath) {
	FILE *file;
	fopen_s(&file, filepath, "rt");
	if (file == NULL) {
		std::cout << "Cannot Open File " << filepath << std::endl;
		return;
	}

	std::vector<vec3> normal;
	std::vector<vec2> texture;
	std::vector<vec3u> pair;
	vec3 temp_v3;
	vec3u pair1, pair2, pair3;
	vec2 temp_v2;

	while (true) {
		char header[128];
		int eof = fscanf(file, "%s", header);
		if (eof == EOF)
			break;
		if (strcmp(header, "v") == 0) {
			fscanf(file, "%f %f %f\n", &temp_v3.x, &temp_v3.y, &temp_v3.z);
			m_Verticies.push_back(temp_v3);
		}
		else if (strcmp(header, "vt") == 0) {
			fscanf(file, "%f %f\n", &temp_v2.x, &temp_v2.y);
			texture.push_back(temp_v2);
		}
		else if (strcmp(header, "vn") == 0) {
			fscanf(file, "%f %f %f\n", &temp_v3.x, &temp_v3.y, &temp_v3.z);
			normal.push_back(temp_v3);
		}
		else if (strcmp(header, "f") == 0) {
			if (texture.size() == 0) {
				fscanf(file, "%hd//%hd %hd//%hd %hd//%hd\n", &pair1.x, &pair1.z, &pair2.x, &pair2.z, &pair3.x, &pair3.z);
				pair1.y = pair2.y = pair3.y = 0;
			}
			else {
				fscanf(file, "%hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd\n", &pair1.x, &pair1.y, &pair1.z, &pair2.x, &pair2.y, &pair2.z, &pair3.x, &pair3.y, &pair3.z);
			}
			m_Indicies.push_back(vec3u(pair1.x - 1, pair2.x - 1, pair3.x - 1));
			pair.push_back(pair1);
			pair.push_back(pair2);
			pair.push_back(pair3);
		}
		else {
			char irrelevant[500];
			fgets(irrelevant, 500, file);
		}
	}
	fclose(file);

	m_Textures.resize(m_Verticies.size());
	m_Normals.resize(m_Verticies.size());

	if (texture.size() == 0) {
		for (vec3u p : pair) {
			m_Normals[p.x - 1] = normal[p.z - 1];
		}
	}
	else {
		for (vec3u p : pair) {
			m_Normals[p.x - 1] = normal[p.z - 1];
			m_Textures[p.x - 1] = texture[p.y - 1];
		}
	}

	return;
}

void Model::render() {
	VertexArray vao;
	IndexBuffer ibo(&m_Indicies[0].x, m_Indicies.size() * 3);
	vao.addBuffer(new Buffer(&m_Verticies[0].x, m_Verticies.size() * 3, 3), 0);
	vao.addBuffer(new Buffer(&m_Textures[0].x, m_Textures.size() * 2, 2), 1);
	vao.addBuffer(new Buffer(&m_Normals[0].x, m_Normals.size() * 3, 3), 2);

	vao.bind();
	ibo.bind();
	glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
	vao.unbind();
	ibo.unbind();
	return;
}