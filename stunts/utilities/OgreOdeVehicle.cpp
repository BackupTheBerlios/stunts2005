#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeJoint.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeSpace.h"
#include "OgreOdeVehicle.h"
#include "OgreOdeEntityInformer.h"
#include "OgreOdeMass.h"
#include "OgreOdeUtility.h"

#include "OgreOdeVehicle.h"

using namespace OgreOde_Prefab;

const Real Vehicle::Wheel::FudgeFactor = 0.1;

Vehicle::Wheel::Wheel(Vehicle* vehicle,const String& name,const String& mesh,const Vector3& position,Real mass,Space* space):PrefabObject(OgreOde_Prefab::ObjectType_Wheel)
{
	_owner	= vehicle;
	_name	= name;

	_entity	= World::getSingleton().getSceneManager()->createEntity(name + "_Entity",mesh);
	_entity->setCastShadows(true);	

	EntityInformer ei(_entity);
	_radius	= ei.getRadius();

	_node	= World::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(name + "_Node");
	_node->setPosition(position);
	_body	= new Body(name + "_Body");

	_node->attachObject(_entity);
	_node->attachObject(_body);

	_body->setMass(SphereMass(mass,_radius));

	_geometry = new SphereGeometry(_radius,space);
	_geometry->setBody(_body);
	_geometry->setPrefabObject(this);

	_joint	= new OgreOde::VehicleSuspensionJoint();
	_joint->attach(vehicle->getBody(),_body);
	_joint->setAnchor(_body->getPosition());
	_joint->setAxis(Vector3::UNIT_Y);
	_joint->setAdditionalAxis(Vector3::NEGATIVE_UNIT_X);
	_joint->setParameter(Joint::Parameter_FudgeFactor,Vehicle::Wheel::FudgeFactor);
	
	_bouncyness = _friction = _fds = 0.0;

	setSteerFactor(0.0);
	setSteerLimit(0.0);
	setSteerForce(0.0);
	setSteerSpeed(0.0);
			
	setPowerFactor(0.0);
			
	setBrakeFactor(0.0);

	setSteer(0.0);
}

void Vehicle::Wheel::setFactors(Real steer,Real power,Real brake)
{
	setSteerFactor(steer);
	setPowerFactor(power);
	setBrakeFactor(brake);
}

Real Vehicle::Wheel::getRPM()
{
	return ((_joint)&&(_power_factor > 0.0))?_joint->getParameter(Joint::Parameter_MotorVelocity,2):0.0;
}

Real Vehicle::Wheel::getSteer()
{
	return ((_joint)&&(_steer_factor != 0.0))?_joint->getAngle():0.0;
}

void Vehicle::Wheel::setSteerFactor(Real factor)
{
	_steer_factor = factor;
	adjustJointParameters();
}

void Vehicle::Wheel::setSteerLimit(Real limit)
{
	_steer_limit = limit;
	adjustJointParameters();
}

void Vehicle::Wheel::adjustJointParameters()
{
	if(_joint)
	{
		_joint->setParameter(Joint::Parameter_HighStop,_steer_limit * fabs(_steer_factor));
		_joint->setParameter(Joint::Parameter_LowStop,-(_steer_limit * fabs(_steer_factor)));
	}
}

/* 
According to the ODE docs;

By adjusting the values of ERP and CFM, you can achieve various effects. 
For example you can simulate springy constraints, where the two bodies oscillate 
as though connected by springs. Or you can simulate more spongy constraints, without 
the oscillation. In fact, ERP and CFM can be selected to have the same effect as any 
desired spring and damper constants. If you have a spring constant kp and damping constant kd, 
then the corresponding ODE constants are:

ERP = h kp / (h kp + kd)
CFM = 1 / (h kp + kd)

where h is the stepsize. These values will give the same effect as a spring-and-damper 
system simulated with implicit first order integration.
*/
void Vehicle::Wheel::setSuspension(Real spring_constant,Real damping_constant,Real step_size)
{
	_spring = spring_constant;
	_damping = damping_constant;
	_step = step_size;

	if(_joint)
	{
		_joint->setParameter(Joint::Parameter_SuspensionERP,_step * _spring / (_step * _spring + _damping));
		_joint->setParameter(Joint::Parameter_SuspensionCFM,1.0 / (_step * _spring + _damping));
	}
}

const Vector3& Vehicle::Wheel::getPosition()
{
	return _body->getPosition();
}

void Vehicle::Wheel::setPosition(const Vector3& position)
{
	_body->setPosition(position);
}

void Vehicle::Wheel::update(Real power_force,Real desired_rpm,Real brake_force)
{
	if(!_joint) return;

	Real force = ((power_force * _power_factor) - (brake_force * _brake_factor));

	if(force > 0.0)
	{
		_joint->setParameter(Joint::Parameter_MotorVelocity,desired_rpm,2);
		_joint->setParameter(Joint::Parameter_MaximumForce,force,2);
	}
	else
	{
		_joint->setParameter(Joint::Parameter_MotorVelocity,0.01,2);
		_joint->setParameter(Joint::Parameter_MaximumForce,-force,2);
	}
}

void Vehicle::Wheel::setSteer(Real position)
{
	Real steer_angle = position * _steer_limit;

	// Steer
	if(_steer_limit * _steer_factor != 0.0)
	{
		Real vel = (steer_angle * _steer_factor) - _joint->getAngle();
		vel *= (_steer_speed * fabs(_steer_factor));

		_joint->setParameter(Joint::Parameter_MotorVelocity,vel);
		_joint->setParameter(Joint::Parameter_MaximumForce,_steer_force);
	}
}

void Vehicle::Wheel::snapOff()
{
	delete _joint;
	_joint = 0;
}

Vehicle::Wheel::~Wheel()
{
	delete _joint;
	delete _body;
	delete _geometry;

	World::getSingleton().getSceneManager()->getRootSceneNode()->removeAndDestroyChild(_node->getName());
	World::getSingleton().getSceneManager()->removeEntity(_entity->getName());
}

Vehicle::Vehicle(const String& name,const String& mesh,Real mass,const Vector3& offset,Geometry* geometry):PrefabObject(OgreOde_Prefab::ObjectType_Vehicle)
{
	_name	= name;

	_entity	= World::getSingleton().getSceneManager()->createEntity(name + "_Entity",mesh);
	_entity->setCastShadows(true);
	_node	= World::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(name + "_Node");
	_body	= new Body(name + "_Body");

	_trans_node = _node->createChildSceneNode("Trans_" + _node->getName());
	_trans_node->attachObject(_entity);
	_node->attachObject(_body);

	EntityInformer ei(_entity);
	Vector3 box = ei.getSize();
	
	BoxMass box_mass(mass,Vector3(box.x,box.y,box.z));
	_body->setMass(box_mass);

	_space 	= new SimpleSpace(World::getSingleton().getDefaultSpace());
	_space->setInternalCollisions(false);
	_space->setAutoCleanup(false);

	_offset = -offset;

	if(!geometry) _geometry = new BoxGeometry(box);
	else _geometry = geometry;
	_geometry->setPrefabObject(this);

	_transform = new TransformGeometry(_space);
	_transform->setEncapsulatedGeometry(_geometry);
	_transform->setBody(_body);
	_geometry->setPosition(_offset);

	_trans_node->setPosition(_offset);

	_engine = new Vehicle::Engine();
}

Vehicle::Wheel* Vehicle::addWheel(const String& mesh,const Vector3& position,Real mass)
{
	Vehicle::Wheel* wheel = new Vehicle::Wheel(this,_name + "_Wheel" + StringConverter::toString((unsigned int)_wheels.size() + 1),mesh,position + _body->getPosition() + _offset,mass,_space);
	_wheels.push_back(wheel);
	return wheel;
}

const Vector3& Vehicle::getPosition()
{
	return _body->getPosition();
}

const Quaternion& Vehicle::getOrientation()
{
	return _body->getOrientation();
}

void Vehicle::setPosition(const Vector3& position)
{
	for(std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		Vector3 diff = (*i)->getPosition() - _body->getPosition();
		(*i)->setPosition(position + diff);
	}
	_body->setPosition(position);
}

void Vehicle::setSuspension(Real spring_constant,Real damping_constant,Real step_size)
{
	for(std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		(*i)->setSuspension(spring_constant,damping_constant,step_size);
	}
}

void Vehicle::setInputs(bool left,bool right,bool throttle,bool brake)
{
	_engine->setInputs(throttle,brake);
	setSteering(0.0 + ((right)?1.0:0.0) - ((left)?1.0:0.0));
}

void Vehicle::setInputs(Real steering_position,Real throttle_brake)
{
	_engine->setInputs(throttle_brake);
	setSteering(steering_position);
}

void Vehicle::setInputs(Real steering_position,Real throttle_position,Real brake_position)
{
	_engine->setInputs(throttle_position,brake_position);
	setSteering(steering_position);
}

void Vehicle::setSteering(Real steering_position)
{
	for(std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		(*i)->setSteer(steering_position);
	}
}

Real Vehicle::getVelocity()
{
	return _body->getLinearVelocity().length();
}

void Vehicle::update(Real time)
{
	std::vector<Vehicle::Wheel*>::iterator b = _wheels.begin();
	std::vector<Vehicle::Wheel*>::iterator e = _wheels.end();

	Real rpm = FLT_MIN;

	for(std::vector<Vehicle::Wheel*>::iterator i = b;i != e;i++)
	{
		rpm = std::max(rpm,(*i)->getRPM());
	}

	_engine->update(time);
	Real power = _engine->getPowerAtRPM(rpm);
	Real desired_rpm = _engine->getDesiredRPM();
	Real brake = _engine->getBrakeForce();

	for(std::vector<Vehicle::Wheel*>::iterator i = b;i != e;i++)
	{
		(*i)->update(power,desired_rpm,brake);
	}
}

Vehicle::Wheel* Vehicle::findWheel(Geometry* geometry)
{
	for(std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		if((*i)->getGeometry() == geometry) return (*i);
	}
	return 0;
}

Vehicle::~Vehicle()
{
	// World::getSingleton().unregisterVehicle(_name);

	for(std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++) delete (*i);

	delete _body;
	delete _geometry;
	delete _transform;
	delete _space;
	delete _engine;

	_node->removeAndDestroyChild(_trans_node->getName());
	World::getSingleton().getSceneManager()->getRootSceneNode()->removeAndDestroyChild(_node->getName());
	World::getSingleton().getSceneManager()->removeEntity(_entity->getName());
}

Vehicle::Engine::Engine()
{
	setInputs(false,false);
	setRevLimit(40.0);
	_torque_curve = 0;
	setTorque(0.5,5.0);
	setBrakeForce(500.0);
}

Real Vehicle::Engine::getPowerAtRPM(Real rpm)
{
	_wheel_rpm = rpm;

	Real pos = getRevFactor() * (_curve_length - 1);
	unsigned int i = (unsigned int)pos;
	Real dx = pos - ((Real)i);
	return _torque_curve[i] + (dx * (_torque_curve[i+1] - _torque_curve[i]));
}

Real Vehicle::Engine::getBrakeForce()
{
	return _brake_force * _brake_position;
}

void Vehicle::Engine::setInputs(Real throttle_position,Real brake_position)
{
	_throttle_position = throttle_position;
	_brake_position = _brake_position;
}

void Vehicle::Engine::setInputs(Real throttle_brake)
{
	_throttle_position = std::max((Real)0.0,throttle_brake);
	_brake_position = std::max((Real)0.0,-throttle_brake);
}

void Vehicle::Engine::setInputs(bool throttle,bool brake)
{
	_throttle_position = (throttle)?1.0:0.0;
	_brake_position = (brake)?1.0:0.0;
}

// Manual gearbox
void Vehicle::Engine::changeUp()
{
	if (this->_current_gear.first == '1') this->setGear('2');
	else if (this->_current_gear.first == '2') this->setGear('3');
	else if (this->_current_gear.first == '3') this->setGear('4');
	else if (this->_current_gear.first == '4') this->setGear('5');
	else if (this->_current_gear.first == '5') this->setGear('6');
}

void Vehicle::Engine::changeDown()
{
	if (this->_current_gear.first == '2') this->setGear('1');
	else if (this->_current_gear.first == '3') this->setGear('2');
	else if (this->_current_gear.first == '4') this->setGear('3');
	else if (this->_current_gear.first == '5') this->setGear('4');
	else if (this->_current_gear.first == '6') this->setGear('5');

}

void Vehicle::Engine::setGear(char code)
{
	std::pair<char, Real> gearPair;
	
	
	for (int i = 0; i < _gears.size(); i++)
	{
		// Get current gear from vector
		gearPair = _gears[i];
		
		// Check if this is the right gear and save it
		if (gearPair.first == code)
		{
			 _current_gear = _gears[i];
			 _current_gear_ratio = -7.5/_current_gear.second;
		};
		
	};
	
}

// Information functions
Real Vehicle::Engine::getDesiredRPM()
{
	return _rev_limit * _throttle_position * _current_gear_ratio / 1.1;
}

Real Vehicle::Engine::getRevFactor()
{
	return _wheel_rpm * _inv_rev_limit;
}

char Vehicle::Engine::getGearCode()
{	
	return _current_gear.first;
}

// Engine parameters
void Vehicle::Engine::setRevLimit(Real rpm)
{
	_rev_limit = rpm * _current_gear_ratio * 20;
	_inv_rev_limit = 1.0 / _rev_limit;
}

// Set a custom torque curve
void Vehicle::Engine::setTorque(Real* torque,unsigned int samples)
{
	assert((samples > 1)&&("Need more than one sample for setTorque"));

	delete[] _torque_curve;
	_curve_length = samples;
	_torque_curve = new Real[_curve_length + 1];

	for(unsigned int i = 0;i < samples;i++) _torque_curve[i] = torque[i];
	_torque_curve[_curve_length] = torque[samples - 1];
}

// Set a linear torque curve, minimum should be greater than zero, 
// or the vehicle won't go anywhere!
void Vehicle::Engine::setTorque(Real minimum,Real maximum)
{
	Real curve[] = {minimum,maximum};
	setTorque(curve,2);
}

// Set a constant torque "curve"
void Vehicle::Engine::setTorque(Real constant)
{
	setTorque(constant,constant);
}

// Braking parameters
void Vehicle::Engine::setBrakeForce(Real force)
{
	_brake_force = force;
}

void Vehicle::Engine::update(Real time)
{
}

// Drivetrain parameters  
void Vehicle::Engine::addGear(Real ratio, char code) 
{
	std::pair<char,  Real> gearPair;  
	gearPair =  std::make_pair(code, ratio);  
	this->_gears.push_back(gearPair); 
}

void Vehicle::Engine::setFinalDrive(Real ratio)
{
}

void Vehicle::Engine::setAutoShiftRevs(Real up,Real down)
{
}

void Vehicle::Engine::setAutoBox(bool automatic)
{
	this->_auto_box = automatic;
}

Vehicle::Engine::~Engine()
{
	delete[] _torque_curve;
}

void Vehicle::Wheel::setupTyreContact(OgreOde::Contact* contact)
{
	Quaternion vehicle_orient = _owner->getBody()->getOrientation();

	Vector3 wheel_up = vehicle_orient * Vector3::UNIT_Y;
	Quaternion wheel_rot = Quaternion(Radian(getSteer()),wheel_up);
	
	Vector3 wheel_forward = wheel_rot * (vehicle_orient * Vector3::UNIT_Z);
	Vector3 wheel_velocity = _body->getLinearVelocity();

	contact->setFrictionMode(Contact::Flag_BothFrictionPyramids);
	contact->setBouncyness(_bouncyness);
	contact->setCoulombFriction(_friction);
	contact->setFirstFrictionDirection(wheel_forward);
	contact->setAdditionalFDS(wheel_velocity.length() * _fds);
}

bool Vehicle::handleTyreCollision(OgreOde::Contact* contact)
{
	Geometry *geom = contact->getFirstGeometry();

	if((geom->getPrefabObject()) && (geom->getPrefabObject()->getObjectType() == OgreOde_Prefab::ObjectType_Wheel))
	{
		((OgreOde_Prefab::Vehicle::Wheel*)geom->getPrefabObject())->setupTyreContact(contact);
		return true;
	}
	else
	{
		geom = contact->getSecondGeometry();
		if((geom->getPrefabObject()) && (geom->getPrefabObject()->getObjectType() == OgreOde_Prefab::ObjectType_Wheel))
		{
			((OgreOde_Prefab::Vehicle::Wheel*)geom->getPrefabObject())->setupTyreContact(contact);
			return true;
		}
	}
	return false;
}

Vehicle::Vehicle(const String& name,Geometry* geometry):PrefabObject(OgreOde_Prefab::ObjectType_Vehicle)
{
	_name = name;

	_space = new SimpleSpace(World::getSingleton().getDefaultSpace());
	_space->setInternalCollisions(false);
	_space->setAutoCleanup(false);

	_engine = new Vehicle::Engine();

	_entity = 0;

	_node = World::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(name + "_Node");
	_body = new Body(name + "_Body");

	_trans_node = _node->createChildSceneNode("Trans_" + _node->getName());
	_node->attachObject(_body);

	_geometry = geometry;
	if(_geometry) _geometry->setPrefabObject(this);
	_transform = 0;
}

#include <tinyxml.h>

void Vehicle::load(const String &filename,const String &definition_name)
{
	String my_name = definition_name;
	if(my_name == StringUtil::BLANK) my_name = _name;

	DataStreamPtr file = ResourceGroupManager::getSingleton().openResource(filename);
	TiXmlDocument *doc = new TiXmlDocument(filename.c_str());
	doc->Parse(file->getAsString().c_str());
	if(doc->Error())
	{
		throw new Exception(doc->ErrorId(),doc->ErrorDesc(),__FILE__,((char*)(file->getName().c_str())),doc->ErrorRow());
	}

	const TiXmlElement *root = doc->RootElement();
	for(const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	{
		if((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(),"vehicle")))
		{
			const TiXmlElement *vehicle = (const TiXmlElement*)child;
			if(vehicle->Attribute("name") && (!strcmp(vehicle->Attribute("name"),my_name.c_str())))
			{
				for(const TiXmlNode *tag = child->FirstChild(); tag; tag = tag->NextSibling() )
				{
					if(tag->Type() == TiXmlNode::ELEMENT)
					{
						const TiXmlElement *element = (const TiXmlElement*)tag;

						if(!strcmp(tag->Value(),"body"))
						{
							if(element->Attribute("mesh"))
							{
								_entity = World::getSingleton().getSceneManager()->createEntity(_name + "_Entity",element->Attribute("mesh"));
								_entity->setCastShadows(true);

								_trans_node->attachObject(_entity);

								EntityInformer ei(_entity);
								Vector3 box = ei.getSize();

								if(!_geometry) _geometry = new BoxGeometry(box);
								_geometry->setPrefabObject(this);

								_transform = new TransformGeometry(_space);
								_transform->setEncapsulatedGeometry(_geometry);
								_transform->setBody(_body);
							}

							for(const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
							{
								if(sub_tag->Type() == TiXmlNode::ELEMENT)
								{
									const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

									if(_entity && (!strcmp(sub_tag->Value(),"mass")))
									{
										double tmp;
										if(sub_element->Attribute("value",&tmp))
										{
											EntityInformer ei(_entity);
											Vector3 box = ei.getSize();

											BoxMass box_mass((Real)tmp,Vector3(box.x,box.y,box.z));
											_body->setMass(box_mass);
										}

										Vector3 offset = Vector3::ZERO;

										if(sub_element->Attribute("x",&tmp)) offset.x = (Real)tmp; 
										if(sub_element->Attribute("y",&tmp)) offset.y = (Real)tmp; 
										if(sub_element->Attribute("z",&tmp)) offset.z = (Real)tmp; 
							
										_offset = -offset;
										if(_geometry) _geometry->setPosition(_offset);
										_trans_node->setPosition(_offset);
									}
								}
							}
						}
						else if(!strcmp(tag->Value(),"wheel"))
						{
							Vehicle::Wheel *wheel = 0;
							double x,y,z,mass;
							
							if(element->Attribute("mesh"))
							{
								wheel = addWheel(element->Attribute("mesh"),Vector3((element->Attribute("x",&x))?(Real)x:0.0,(element->Attribute("y",&y))?(Real)y:0.0,(element->Attribute("z",&z))?(Real)z:0.0),(element->Attribute("mass",&mass))?(Real)mass:1.0);
							}

							if(wheel)
							{
								for(const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
								{
									if(sub_tag->Type() == TiXmlNode::ELEMENT)
									{
										const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

										if(!strcmp(sub_tag->Value(),"steer"))
										{
											double tmp;
											if(sub_element->Attribute("factor",&tmp)) wheel->setSteerFactor(tmp);
											if(sub_element->Attribute("limit",&tmp)) wheel->setSteerLimit(tmp);
											if(sub_element->Attribute("force",&tmp)) wheel->setSteerForce(tmp);
											if(sub_element->Attribute("speed",&tmp)) wheel->setSteerSpeed(tmp);
										}
										else if(!strcmp(sub_tag->Value(),"power"))
										{
											double tmp;
											if(sub_element->Attribute("factor",&tmp)) wheel->setPowerFactor(tmp);
										}
										else if(!strcmp(sub_tag->Value(),"brake"))
										{
											double tmp;
											if(sub_element->Attribute("factor",&tmp)) wheel->setBrakeFactor(tmp);
										}
										else if(!strcmp(sub_tag->Value(),"contact"))
										{
											double bouncyness,friction,fds;
											wheel->setContact((sub_element->Attribute("bouncyness",&bouncyness))?(Real)bouncyness:0.0,(sub_element->Attribute("friction",&friction))?(Real)friction:0.0,(sub_element->Attribute("fds",&fds))?(Real)fds:0.0);
										}
										else if(!strcmp(sub_tag->Value(),"suspension"))
										{
											double spring,damping,step;
											setSuspension((sub_element->Attribute("spring",&spring))?(Real)spring:0.0,(sub_element->Attribute("damping",&damping))?(Real)damping:0.0,(sub_element->Attribute("step",&step))?(Real)step:0.0);
										}
									}
								}
							}
						}
						else if(!strcmp(tag->Value(),"suspension"))
						{
							double spring,damping,step;
							setSuspension((element->Attribute("spring",&spring))?(Real)spring:0.0,(element->Attribute("damping",&damping))?(Real)damping:0.0,(element->Attribute("step",&step))?(Real)step:0.0);
						}
						else if(!strcmp(tag->Value(),"engine"))
						{
							double tmp;

							if(element->Attribute("redline",&tmp)) _engine->setRevLimit((Real)tmp);
							if(element->Attribute("brake",&tmp)) _engine->setBrakeForce((Real)tmp);

							for(const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
							{
								if(sub_tag->Type() == TiXmlNode::ELEMENT)
								{
									const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

									if(!strcmp(sub_tag->Value(),"torque"))
									{
										double min_torque,max_torque;
										_engine->setTorque((sub_element->Attribute("min",&min_torque))?min_torque:0.0,(sub_element->Attribute("max",&max_torque))?max_torque:0.0);
									}
								}
							}
						}
					}
				}
				break;
			}
		}
	}

	delete doc;
}
