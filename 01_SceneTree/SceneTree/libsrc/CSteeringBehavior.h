#ifndef _CSTEERINGBEHAVIOR_H_
#define _CSTEERINGBEHAVIOR_H_ 

#include <SFML/Graphics.hpp>
#include <math.h>
class CActor;
class CSteeringBehavior {

  public :
	static inline sf::Vector2f Normalize(sf::Vector2f vec2) 
	{
    sf::Vector2f vec2Result = vec2;
    float fLength = CSteeringBehavior::Length(vec2);
    if (fLength > 0.0f) {
      vec2Result *= 1.0f/fLength;
    }

		return vec2Result;
	}

	static inline float Length(sf::Vector2f vec) 
	{
		return (float)sqrt(vec.x * vec.x + vec.y * vec.y);
	}



  static inline sf::Vector2f Seek(
      sf::Vector2f& vec2Pos, 
      sf::Vector2f& vec2Target, 
      sf::Vector2f& vec2CurVel,
      float fMaxVelocity,
      float fMaxForce
      ) {
    sf::Vector2f vec2DesiredV = vec2Target - vec2Pos;

    sf::Vector2f vec2SteeringForce;

    vec2DesiredV = CSteeringBehavior::Normalize(vec2DesiredV);
    vec2DesiredV *= fMaxVelocity;

    vec2SteeringForce = vec2DesiredV - vec2CurVel;
    vec2SteeringForce /= fMaxVelocity;
    vec2SteeringForce *= fMaxForce;

    return vec2SteeringForce;
  }
};
#endif /* ifndef _CSTEERINGBEHAVIOR_H_ */
