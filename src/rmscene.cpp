#include "rmscene.hpp"

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
			s[i] = spheres[i].sphere;
			sc[i] = vec4(spheres[i].color.r / 255, spheres[i].color.g / 255, spheres[i].color.b / 255, spheres[i].color.a / 255);
		}


		vec4* uSpheres = s.data();
		vec4* uColor = sc.data();

		shader->setUniformArray("sphere", uSpheres, size);
		shader->setUniform("numSpheres", size);
		shader->setUniformArray("sphereCol", uColor, size);
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
			cc[i] = vec4(capsules[i].color.r / 255, capsules[i].color.g / 255, capsules[i].color.b / 255, capsules[i].color.a / 255);

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
			pc[i] = vec4(planes[i].color.r / 255, planes[i].color.g / 255, planes[i].color.b / 255, planes[i].color.a / 255);

		}
		vec3* upp = pp.data();
		vec3* upn = pn.data();
		vec4* upc = pc.data();

		shader->setUniformArray("planeP", upp, size);
		shader->setUniformArray("planeN", upn, size);
		shader->setUniform("numPlanes", size);
		shader->setUniformArray("planeCol", upc, size);
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
