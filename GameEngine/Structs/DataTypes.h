#pragma once


struct Float2
{
	float x;
	float y;

	Float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Float2() {};
};

struct Float3
{
	float x;
	float y;
	float z;

	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Float3() {};
};

struct Float4
{
	float x;
	float y;
	float z;
	float w;

	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Float4() {};
};

struct INT4
{
	int x;
	int y;
	int z;
	int w;

	INT4(int x, int y, int z, int w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	INT4() {};
};

struct UINT4
{
	unsigned int x;
	unsigned y;
	unsigned z;
	unsigned w;

	UINT4(unsigned x, unsigned y, unsigned z, unsigned w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	UINT4() {};
};


struct Double3
{
	double x;
	double y;
	double z;

	Double3() {};

	Double3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};




struct Vertex
{
	Float3 position;
	Float3 normal;
	Float2 uv;
	Float2 biTangent;
	Float2 tangent;

	Vertex(Float3 pos, Float3 normal, Float2 uv, Float2 tangent, Float2 biTangent)
	{
		this->position.x = pos.x;
		this->position.y = pos.y;
		this->position.z = pos.z;

		this->normal.x = normal.x;
		this->normal.y = normal.y;
		this->normal.z = normal.z;

		this->uv.x = uv.x;
		this->uv.y = uv.y;

		this->tangent.x = tangent.x;
		this->tangent.y = tangent.y;

		this->biTangent.x = biTangent.x;
		this->biTangent.y = biTangent.y;

	}
	Vertex() {};

};

struct AnimVert
{
	Float3 position;
	Float3 normal;
	Float2 uv;
	Float2 biTangent;
	Float2 tangent;

	Float4 weights;
	UINT4 jointID;

	AnimVert(Float3 pos, Float3 normal, Float2 uv, Float2 tangent, Float2 biTangent, Float4 weights, UINT4 jointID)
	{
		this->position.x = pos.x;
		this->position.y = pos.y;
		this->position.z = pos.z;

		this->normal.x = normal.x;
		this->normal.y = normal.y;
		this->normal.z = normal.z;

		this->uv.x = uv.x;
		this->uv.y = uv.y;

		this->biTangent.x = biTangent.x;
		this->biTangent.y = biTangent.y;

		this->tangent.x = tangent.x;
		this->tangent.y = tangent.y;

		this->weights.x = weights.x;
		this->weights.y = weights.y;
		this->weights.z = weights.z;
		this->weights.w = weights.w;

		this->jointID.x = jointID.x;
		this->jointID.y = jointID.y;
		this->jointID.z = jointID.z;
		this->jointID.w = jointID.w;
	}
	AnimVert() {};

};


struct ProjectileStruct
{
	Float3 position;
	Float3 rotation;
};
