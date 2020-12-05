#include "rmscene.hpp"
#include <fstream>
#include <istream>


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
			sc[i] = vec4(spheres[i].color.x, spheres[i].color.y, spheres[i].color.z, 1.0f);
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
			cc[i] = vec4(capsules[i].color.x, capsules[i].color.y, capsules[i].color.z, 1.0f);

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
			// normalizing color value
			pc[i] = vec4(planes[i].color.x, planes[i].color.y, planes[i].color.z, 1.0f);

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
	std::ofstream file(path + "/" + (name + ".rmsop"));

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

		output += std::to_string(spheres[i].color.x) + " "
			+ std::to_string(spheres[i].color.y) + " "
			+ std::to_string(spheres[i].color.z) + "\n";
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

		output += std::to_string(capsules[i].color.x) + " "
			+ std::to_string(capsules[i].color.y) + " "
			+ std::to_string(capsules[i].color.z) + "\n";
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

		output += std::to_string(planes[i].color.x) + " "
			+ std::to_string(planes[i].color.y) + " "
			+ std::to_string(planes[i].color.z) + "\n";
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
		current.color.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.z = std::stof(line.substr(li, ci));

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
		current.color.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.z = std::stof(line.substr(li, ci));

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
		current.color.x = std::stof(line.substr(0, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.y = std::stof(line.substr(li, ci));
		li = ci + 1;
		ci = line.find(" ", li);
		current.color.z = std::stof(line.substr(li, ci));

		New.planes.push_back(current);
	}

	*this = New;

	return true;
}
