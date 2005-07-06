#ifndef _OGREODEVEHICLE_H_
#define _OGREODEVEHICLE_H_

#include "OgreOde_Prefab.h"

#include <vector>

namespace OgreOde_Prefab 
{
	class _OgreOdeExport_Prefab Vehicle : public PrefabObject
    {
	public:
		class _OgreOdeExport_Prefab Engine
		{
		public:
			Engine();
			~Engine();

			void 		setInputs(Real throttle_position,Real brake_position);
			void 		setInputs(Real throttle_brake);
			void 		setInputs(bool throttle,bool brake);
			
			Real 		getBrakeForce();
			Real 		getPowerAtRPM(Real rpm);
			Real 		getDesiredRPM();
			inline Real 	getCurrentGearRatio() { return this->_current_gear_ratio; };

			void 		update(Real time);

			// Manual gearbox
			void 		changeUp();
			void 		changeDown();
			void 		setGear(char code);

			// Information functions
			Real 		getRevFactor();
			char 		getGearCode();
		
			// Engine parameters
			void 		setRevLimit(Real rpm);
			void 		setTorque(Real* torque,unsigned int samples);
			void 		setTorque(Real constant);
			void 		setTorque(Real minimum,Real maximum);

			// Braking parameters
			void		setBrakeForce(Real force);

			// Drivetrain parameters
			void 		addGear(Real ratio,char code);
			void 		setFinalDrive(Real ratio);
			void 		setAutoShiftRevs(Real up,Real down);
			void 		setAutoBox(bool automatic);

		protected:
			std::vector<std::pair<char,Real> >	_gears;
			unsigned int				_current_gear_num;
			Real					_current_gear_ratio;
			std::pair<char,Real>			_current_gear;
			Real*					_torque_curve;
			unsigned int				_curve_length;
			Real					_rev_limit,_throttle_position,_brake_position;
			Real					_wheel_rpm,_inv_rev_limit;
			Real					_brake_force;
			Real					_final_drive;
			Real					_shift_up,_shift_down;
			bool					_auto_box;
			
		};

		class _OgreOdeExport_Prefab Wheel : public PrefabObject
		{
		public:
			static const Real FudgeFactor;

			Wheel(Vehicle* vehicle,const String& name,const String& mesh,const Vector3& position,Real mass,Space* space);
			~Wheel();

			void 			setFactors(Real steer,Real power,Real brake);
			void 			setContact(Real bouncyness,Real friction,Real fds)
			{
				_bouncyness 	= bouncyness;
				_friction 	= friction;
				_fds 		= fds;
			}

			void 			setSteerFactor(Real factor);
			void 			setSteerLimit(Real limit);
			void 			setSteerForce(Real force)	{ _steer_force = force;		}
			void 			setSteerSpeed(Real speed)	{ _steer_speed = speed;		}
			void 			setSteer(Real position);
			
			void 			setPowerFactor(Real power)	{ _power_factor = power;	}
			void 			setBrakeFactor(Real brake)	{ _brake_factor = brake;	}

			void 			setSuspension(Real spring_constant,Real damping_constant,Real step_size);

			const Vector3& 		getPosition();
			const Quaternion& 	getOrientation();
			void 			setPosition(const Vector3& position);

			Real 			getRPM();
			Real 			getSteer();

			void 			update(Real power_force,Real desired_rpm,Real brake_force);
	
			SceneNode* 		getSceneNode()			{ return _node;			}
			Entity*			getEntity()			{ return _entity;		}
			Body* 			getBody()			{ return _body;			}
			Geometry* 		getGeometry()			{ return _geometry;		}

			Real 			getRadius()			{ return _radius;		}

			void 			snapOff();

			void 			setupTyreContact(OgreOde::Contact* contact);

		protected:
			void adjustJointParameters();

			SceneNode*				_node;
			Entity*					_entity;
			Real					_steer_factor,_steer_limit,_steer_force,_steer_speed;
			Real					_power_factor;
			Real					_brake_factor;
			Real					_spring,_damping,_step;
			Real					_bouncyness,_friction,_fds;
			Real					_radius;
			Body*					_body;
			Geometry*				_geometry;
			VehicleSuspensionJoint*	_joint;
			String					_name;
			Vehicle*				_owner;
		};

	public:
		Vehicle(const String& name,const String& mesh,Real mass,const Vector3& offset = Vector3::ZERO,Geometry* geometry = 0);
		Vehicle(const String& name,Geometry* geometry = 0);
		~Vehicle();

		Vehicle::Wheel* 	addWheel(const String& mesh,const Vector3& position,Real mass);
		
		const Vector3& 		getPosition();
		const Quaternion& 	getOrientation();
		void 			setPosition(const Vector3& position);

		void 			setSuspension(Real spring_constant,Real damping_constant,Real step_size);
		
		void 			setInputs(Real steering,Real throttle,Real brake);
		void 			setInputs(Real steering,Real throttle_brake);
		void 			setInputs(bool left,bool right,bool throttle,bool brake);

		void 			setSteering(Real steering_position);

		const String& 		getName(){return _name;}

		void 			update(Real time);

		Entity*			getEntity(){return _entity;}
		Body*			getBody(){return _body;}
		SceneNode* 		getSceneNode(){return _node;}
		Vehicle::Wheel* 	getWheel(unsigned int i){return _wheels[i];}
		Geometry* 		getGeometry(){return _transform;}
		Vehicle::Wheel* 	findWheel(Geometry* geometry);
		Engine* 		getEngine(){return _engine;}

		Real 			getVelocity();
		
		static bool		 handleTyreCollision(OgreOde::Contact* contact);

		void			load(const String &filename,const String &definition_name = StringUtil::BLANK);

	protected:
		SceneNode*				_node;
		SceneNode*				_trans_node;
		Entity*					_entity;
		Body*					_body;
		TransformGeometry*			_transform;
		Geometry*				_geometry;
		std::vector<Vehicle::Wheel*>		_wheels;
		String					_name;
		SimpleSpace*				_space;
		Vector3					_offset;
		Engine*					_engine;
	};
}

#endif

