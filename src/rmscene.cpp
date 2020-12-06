#include "rmscene.hpp"
#include <fstream>
#include <istream>
#include <iostream>


RMscene::RMscene()
{
}

void RMscene::sendToShader(sf::Shader* shader)
{
	int size;
	// spheres
	size = spheres.size();
	if (size > 0)
	{
		std::vector<vec4> s(size, vec4());
		std::vector<vec4> sc(size, vec4());
		for (UINT i = 0; i < size; i++)
		{
			s[i] = spheres[i].s;
			sc[i] = vec4(spheres[i].color.r / 255.0f, spheres[i].color.g / 255.0f, spheres[i].color.b / 255.0f, 1.0f);
		}


		vec4* uSpheres = s.data();
		vec4* uColor = sc.data();

		shader->setUniformArray("sphere", uSpheres, size);
		shader->setUniform("numSpheres", size);
		shader->setUniformArray("sphereCol", uColor, size);
	}
	else
	{
		shader->setUniformArray("sphere", &vec4(), 0);
		shader->setUniform("numSpheres", 0);
		shader->setUniformArray("sphereCol", &vec4(), 0);
	}

	// capsules
	size = capsules.size();
	if (size > 0)
	{
		std::vector<vec3> ca(size, vec3());
		std::vector<vec3> cb(size, vec3());
		std::vector<float> cr(size, 0);
		std::vector<vec4> cc(size, vec4());
		for (UINT i = 0; i < size; i++)
		{
			ca[i] = capsules[i].a;
			cb[i] = capsules[i].b;
			cr[i] = capsules[i].r;
			// normalizing color value
			cc[i] = vec4(capsules[i].color.r / 255.0f, capsules[i].color.g / 255.0f, capsules[i].color.b / 255.0f, 1.0f);

		}

		vec3* uca = ca.data();
		vec3* ucb = cb.data();
		float* ucr = cr.data();
		vec4* ucc = cc.data();

		shader->setUniformArray("capsuleA", uca, size);
		shader->setUniformArray("capsuleB", ucb, size);
		shader->setUniformArray("capsuleR", ucr, size);
		shader->setUniform("numCaps", size);
		shader->setUniformArray("capsuleCol", ucc, size);
	}
	else
	{
		float* ucr;
		shader->setUniformArray("capsuleA", &vec3(), 0);
		shader->setUniformArray("capsuleB", &vec3(), 0);
		shader->setUniformArray("capsuleR", ucr, 0);
		shader->setUniform("numCaps", 0);
		shader->setUniformArray("capsuleCol", &vec4(), 0);
	}

	//planes
	size = planes.size();
	if (size > 0)
	{
		std::vector<vec3> pp(size, vec3());
		std::vector<vec3> pn(size, vec3());
		std::vector<vec4> pc(size, vec4());
		for (UINT i = 0; i < size; i++)
		{
			pp[i] = planes[i].p;
			pn[i] = planes[i].n;
			pc[i] = vec4((float)planes[i].color.g / 255.0f, (float)planes[i].color.g / 255.0f, (float)planes[i].color.b / 255.0f, 1.0f);
		}
		vec3* upp = pp.data();
		vec3* upn = pn.data();
		vec4* upc = pc.data();

		shader->setUniformArray("planeP", upp, size);
		shader->setUniformArray("planeN", upn, size);
		shader->setUniform("numPlanes", size);
		shader->setUniformArray("planeCol", upc, size);
	}
	else
	{
		shader->setUniformArray("planeP", &vec3(), 0);
		shader->setUniformArray("planeN", &vec3(), 0);
		shader->setUniform("numPlanes", 0);
		shader->setUniformArray("planeCol", &vec4(), 0);
	}

	// cubes
	size = cubes.size();
	if (size > 0)
	{
		std::vector<vec3> cp(size, vec3());
		std::vector<vec3> cs(size, vec3());
		std::vector<vec4> cc(size, vec4());
		for (UINT i = 0; i < size; i++)
		{
			cp[i] = cubes[i].p;
			cs[i] = cubes[i].s;
			cc[i] = vec4((float)cubes[i].color.g / 255.0f, (float)cubes[i].color.g / 255.0f, (float)cubes[i].color.b / 255.0f, 1.0f);
		}
		vec3* ucp = cp.data();
		vec3* ucs = cs.data();
		vec4* ucc = cc.data();

		shader->setUniformArray("cubeP", ucp, size);
		shader->setUniformArray("cubeS", ucs, size);
		shader->setUniform("numCubes", size);
		shader->setUniformArray("cubeCol", ucc, size);
	}
	else
	{
		shader->setUniformArray("cubeP", &vec3(), 0);
		shader->setUniformArray("cubeS", &vec3(), 0);
		shader->setUniform("numCubes", 0);
		shader->setUniformArray("cubeCol", &vec4(), 0);
	}
}

void RMscene::addSphere(RMscene::sphere s)
{
	spheres.push_back(s);
}

void RMscene::addCapsule(RMscene::capsule c)
{
	capsules.push_back(c);
}

void RMscene::addPlane(RMscene::plane p)
{
	planes.push_back(p);
}

void RMscene::changeSphere(UINT i, RMscene::sphere s)
{
	if (i < spheres.size())
		spheres[i] = s;
}

void RMscene::changeCapsule(UINT i, RMscene::capsule c)
{
	if (i < capsules.size())
		capsules[i] = c;
}

void RMscene::changePlane(UINT i, RMscene::plane p)
{
	if (i < planes.size())
		planes[i] = p;
}

void RMscene::removeSphere(UINT i)
{
	if (i < spheres.size())
		spheres.erase(spheres.begin() + i);
}

void RMscene::removeCapsule(UINT i)
{
	if (i < capsules.size())
		capsules.erase(capsules.begin() + i);
}

void RMscene::removePlane(UINT i)
{
	if (i < planes.size())
		planes.erase(planes.begin() + i);
}

bool RMscene::writeToFile(std::string name, std::string path)
{
	std::ofstream file;
	std::string filepath;
	if (path != "")
		filepath += path + "/";
	filepath += name + ".rmsop";
	file.open(filepath, std::ios::out);

	if (!file.good())
		return false;

	std::string output;
	output += "Peters SOP RayMarcing Filformat\n";
	output += "spheres\n";
	output += std::to_string(spheres.size()) + "\n";
	for (UINT i = 0; i < spheres.size(); i++)
	{
		output += spheres[i].name + "\n";

		output += std::to_string(spheres[i].s.x) + " "
			+ std::to_string(spheres[i].s.y) + " "
			+ std::to_string(spheres[i].s.z) + " "
			+ std::to_string(spheres[i].s.w) + "\n";

		output += std::to_string(spheres[i].color.r) + " "
			+ std::to_string(spheres[i].color.g) + " "
			+ std::to_string(spheres[i].color.b) + "\n";
	}
	output += "capsules\n";
	output += std::to_string(capsules.size()) + "\n";
	for (UINT i = 0; i < capsules.size(); i++)
	{
		output += capsules[i].name + "\n";

		output += std::to_string(capsules[i].a.x) + " "
			+ std::to_string(capsules[i].a.y) + " "
			+ std::to_string(capsules[i].a.z) + "\n";

		output += std::to_string(capsules[i].b.x) + " "
			+ std::to_string(capsules[i].b.y) + " "
			+ std::to_string(capsules[i].b.z) + "\n";

		output += std::to_string(capsules[i].r) + "\n";

		output += std::to_string(capsules[i].color.r) + " "
			+ std::to_string(capsules[i].color.g) + " "
			+ std::to_string(capsules[i].color.b) + "\n";
	}
	output += "planes\n";
	output += std::to_string(planes.size()) + "\n";
	for (UINT i = 0; i < planes.size(); i++)
	{
		output += planes[i].name + "\n";

		output += std::to_string(planes[i].p.x) + " "
			+ std::to_string(planes[i].p.y) + " "
			+ std::to_string(planes[i].p.z) + "\n";

		
		output += std::to_string(planes[i].n.x) + " "
			+ std::to_string(planes[i].n.y) + " "
			+ std::to_string(planes[i].n.z) + "\n";

		output += std::to_string(planes[i].color.r) + " "
			+ std::to_string(planes[i].color.g) + " "
			+ std::to_string(planes[i].color.b) + "\n";
	}
	output += "cubes\n";
	output += std::to_string(cubes.size()) + "\n";
	for (UINT i = 0; i < planes.size(); i++)
	{
		output += cubes[i].name + "\n";

		output += std::to_string(cubes[i].p.x) + " "
			+ std::to_string(cubes[i].p.y) + " "
			+ std::to_string(cubes[i].p.z) + "\n";

		output += std::to_string(cubes[i].s.x) + " "
			+ std::to_string(cubes[i].s.y) + " "
			+ std::to_string(cubes[i].s.z) + "\n";

		output += std::to_string(cubes[i].color.r) + " "
			+ std::to_string(cubes[i].color.g) + " "
			+ std::to_string(cubes[i].color.b) + "\n";
	}
	output += "END";

	file << output;
	return true;
}

bool RMscene::readFromFile(std::string path)
{
	RMscene New;
	std::ifstream file;

	file.open(path, std::ios::in);

	if (!file.good())
		return false;

	std::string line;

	std::getline(file, line);
	if (line.find("Peters SOP RayMarcing Filformat") == std::string::npos)
		return false;

	std::getline(file, line);
	std::getline(file, line);
	int numObjects = std::stoi(line);
	int ci = 0, li = 0;
	for (UINT i = 0; i < numObjects; i++)
	{
		sphere current;
		std::getline(file, line);
		current.name = line;
		std::getline(file, line);
		ci = line.find(" ");
		current.s.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.s.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.s.z = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.s.w = std::stof(line.substr(li, ci));

		std::getline(file, line);
		ci = line.find(" ");
		current.color.r = std::stoi(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.g = std::stoi(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.b = std::stoi(line.substr(li, ci));

		New.spheres.push_back(current);
	}
	std::getline(file, line);
	std::getline(file, line);
	numObjects = std::stoi(line);
	for (UINT i = 0; i < numObjects; i++)
	{
		capsule current;
		std::getline(file, line);
		current.name = line;

		std::getline(file, line);
		ci = line.find(" ");
		current.a.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.a.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.a.z = std::stof(line.substr(li, ci));
		
		std::getline(file, line);
		ci = line.find(" ");
		current.b.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.b.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.b.z = std::stof(line.substr(li, ci));

		std::getline(file, line);
		current.r = std::stof(line);

		std::getline(file, line);
		ci = line.find(" ");
		current.color.r = std::stoi(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.g = std::stoi(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.b = std::stoi(line.substr(li, ci));

		New.capsules.push_back(current);
	}

	std::getline(file, line);
	std::getline(file, line);
	numObjects = std::stoi(line);
	for (UINT i = 0; i < numObjects; i++)
	{
		plane current;
		std::getline(file, line);
		current.name = line;

		std::getline(file, line);
		ci = line.find(" ");
		current.p.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.p.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.p.z = std::stof(line.substr(li, ci));

		std::getline(file, line);
		ci = line.find(" ");
		current.n.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.n.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.n.z = std::stof(line.substr(li, ci));

		std::getline(file, line);
		ci = line.find(" ");
		current.color.r = std::stoi(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.g = std::stoi(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.b = std::stoi(line.substr(li, ci));

		New.planes.push_back(current);
	}
	std::getline(file, line);
	std::getline(file, line);
	numObjects = std::stoi(line);
	for (UINT i = 0; i < numObjects; i++)
	{
		cube current;
		std::getline(file, line);
		current.name = line;

		std::getline(file, line);
		ci = line.find(" ");
		current.p.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.p.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.p.z = std::stof(line.substr(li, ci));

		std::getline(file, line);
		ci = line.find(" ");
		current.s.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.s.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.s.z = std::stof(line.substr(li, ci));

		std::getline(file, line);
		ci = line.find(" ");
		current.color.r = std::stoi(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.g = std::stoi(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.b = std::stoi(line.substr(li, ci));

		New.cubes.push_back(current);

	}

	*this = New;

	return true;
}

void RMscene::addCube(RMscene::cube c)
{
	cubes.push_back(c);
}

void RMscene::changeCube(UINT i, RMscene::cube c)
{
	if (i < cubes.size())
		cubes[i] = c;
}

void RMscene::removeCube(UINT i)
{
	if (i < cubes.size())
		cubes.erase(cubes.begin() + i);
}
