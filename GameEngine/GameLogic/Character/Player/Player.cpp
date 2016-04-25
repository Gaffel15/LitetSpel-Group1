#include "Player.h"

inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}
inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, float b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;

	return result;
}

inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 a, Vec3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}
inline float get_degrees(float radian)
{

	return (radian * 180) / XM_PI;
}




Player::Player()
{
	this->projectileSystem = new ProjectileSystem;

}


Player::~Player()
{

	delete this->projectileSystem;

}

void Player::Initialize()
{
	graphics			 = Graphics::GetInstance();

	this->position		 = XMFLOAT3(-5.0f, Y_OFFSET, -5.0f);
	this->rotation		 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	VelocityMax = 4.0f;
	slowTimer = 0;
	radius				 = 1.0f;
	DoT = 0.0f;
	DoTDur = 0.0f;
	health = 100.0f;
	projectileSystem->Initialize();
}

void Player::Release()
{
	projectileSystem->Release();
}

void Player::Update(double deltaTime, XMFLOAT3 direction)
{
	if (VelocityMax == 0.2f)
	{
		slowTimer += (float)deltaTime;
		
	}
	if (slowTimer > 3.0f)
	{
		VelocityMax = 4.0f;
		slowTimer = 0.0f;
	}
	
	if (DoT != 0)
	{
		DoTDur += (float)deltaTime;
	}
	if (DoTDur > 2)
	{
		DoT = 0.0f;
		DoTDur = 0.0f;
	}

	health -= DoT;
	if (health <= 0)
	{

		MessageBox(0, L"You Died",
		L"Continue", MB_OK);
		health = 100.0f;
	}
	
	this->direction	 = direction;
	

#pragma region Calculate movement

	velocity.x		 += acceleration.x * (float)deltaTime - velocity.x * fallOfFactor * (float)deltaTime;
	velocity.y		  = 0.0f;
	velocity.z		 += acceleration.z * (float)deltaTime - velocity.z * fallOfFactor * (float)deltaTime;
	

	float currentVelo = velocity.Length();

	if (currentVelo > VelocityMax)
	{

		Vec3 normalizer			= velocity.Normalize();
		normalizer				= normalizer * VelocityMax;
		velocity				= normalizer;
	}

	if (currentVelo > 0.05f)
	{
		position.x				+= velocity.x;
		position.y				 = Y_OFFSET;
		position.z				+= velocity.z;


	}


	acceleration				 = Vec3(0.0f, 0.0f, 0.0f); //reset acceleration for next frame
#pragma endregion
	


#pragma region Calculate  rotation of mesh

	XMVECTOR mouseDirection = XMVectorSet(direction.x, 0.0f, direction.z, 0.0f);
	XMVECTOR meshDirection = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);


	//Calculate angle between meshDir and shotDir
	float cosAngle = XMVector3Dot(mouseDirection, meshDirection).m128_f32[0];
	float angle = acos(cosAngle);
	float degrees = get_degrees(angle);
	////////////////////////////////////////////////////

	if (direction.x < 0)
		degrees = -degrees;

	rotation.y = degrees;

	renderInfo = { position,rotation };
#pragma endregion
	

projectileSystem->UpdateProjectiles(deltaTime);
}



void Player::Render()
{
	graphics->QueueRender(&renderInfo);
	projectileSystem->Render();
}

void Player::Move(MovementDirection* dir, int keyAmount, double deltaTime)
{
	
	for (int i = 0; i < keyAmount; i++)
	{
		if (dir[i] == UP)
		{
			acceleration.z = maxAcceleration;
		}

		if (dir[i] == DOWN)
		{
			acceleration.z = -maxAcceleration;
		}
		if (dir[i] == LEFT)
		{
			acceleration.x = -maxAcceleration;
		}
		if (dir[i] == RIGHT)
		{
			acceleration.x = maxAcceleration;
		}

		float len = acceleration.Length();

		if (len > maxAcceleration)
		{
			Vec3 normalizer	  = acceleration.Normalize();
			normalizer		  = normalizer * maxAcceleration;
			acceleration	  = normalizer;



		}
	}


}

void Player::Shoot(InputKeys input, double deltaTime)
{
	
	if (input == MOUSE_LEFT)
	{
		projectileSystem->FireProjectile(this->position, direction);
	}
	else if (input == KEY_SPACE)
	{
		projectileSystem->FireProjectile(this->position, direction);
	}


}

float Player::GetHealth()
{
	return this->health;
}

void Player::SetHealth(float health)
{
	this->health = health;
}