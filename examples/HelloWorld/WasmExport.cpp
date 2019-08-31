/*
*/

#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include <vector>

#ifdef WEBASM
#define WASM_EXP __attribute__((visibility("default")))
#define __BTWASM_SYSCALL_NAME(name) \
	__attribute__((__import_module__("BTJSRT"), __import_name__(#name)))

#else 
#define WASM_EXP
#define __BTWASM_SYSCALL_NAME(name)
#endif

std::vector<int> gAllChangedMotionSate;	// 这里保存的id是真的id，给js索引用的，并不是指针

extern "C"{
	int __cxa_begin_catch(int a) { return 0; }
	void WASM_EXP test(){
	}
	// 导入函数
	void jslogs(const char* str) __BTWASM_SYSCALL_NAME(logs);
	void jslog(const char* str, int len, float f1, float f2, float f3) __BTWASM_SYSCALL_NAME(log);
}

class JSBtRigidBody : public btRigidBody{
public:
	int jsID = 0;
	JSBtRigidBody(const btRigidBodyConstructionInfo& constructionInfo) : btRigidBody(constructionInfo){
	}
};

class bodyMotionState:public btDefaultMotionState{
public:
	JSBtRigidBody* pBody = nullptr;

	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) {
		btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
		if (pBody){
			gAllChangedMotionSate.push_back(pBody->jsID);
		}
	}
};

/*
*  world 开始更新对象前执行的操作
*/
void wasmInternalTickCallback(btDynamicsWorld* world, btScalar timeStep) {
	gAllChangedMotionSate.clear();
	jslogs("world pretick");
}


extern "C"{

	// 导出函数
	btScalar gCheck = 0.125;//两边的约定数字
	int WASM_EXP getCheckPtr() {
		return (int)&gCheck;
	}
	////// world //////
	int WASM_EXP createWorld()
	{
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		btDiscreteDynamicsWorld* pWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		pWorld->setInternalTickCallback(wasmInternalTickCallback, 0, true);
		return (int)pWorld;
	}

	void WASM_EXP deleteWorld(int world)
	{
		btDiscreteDynamicsWorld* pWorld = (btDiscreteDynamicsWorld*)world;
		delete pWorld;
	}

	void WASM_EXP world_setG(int world, float x, float y, float z)
	{
		btDiscreteDynamicsWorld* pWorld = (btDiscreteDynamicsWorld*)world;
		btVector3 g(x, y, z);
		pWorld->setGravity(g);
	}

	void WASM_EXP world_addBody(int world, int body)
	{
		btDiscreteDynamicsWorld* pWorld = (btDiscreteDynamicsWorld*)world;
		pWorld->addRigidBody((btRigidBody*)body);
	}

	void WASM_EXP world_removeBody(int world, int body)
	{
		btDiscreteDynamicsWorld* pWorld = (btDiscreteDynamicsWorld*)world;
		pWorld->removeRigidBody((btRigidBody*)body);
	}

	void WASM_EXP world_step(int world, float fixstep, float dt, int maxinterp)
	{
		btDiscreteDynamicsWorld* pWorld = (btDiscreteDynamicsWorld*)world;
		pWorld->stepSimulation(dt, maxinterp, fixstep);
	}

	struct  AllMotionStateInfo{
		int num;
		int ptr;
	}gAllMotionStateInfo;

	int WASM_EXP world_getAllMotionState() {
		gAllMotionStateInfo.num = gAllChangedMotionSate.size();
		gAllMotionStateInfo.ptr = (int)&(gAllChangedMotionSate[0]);
		return (int)&gAllMotionStateInfo;
	}
	////// shape //////
	// m 是因为要计算转动惯量
	int WASM_EXP createBox(float x, float y, float z, float m)
	{
		btCollisionShape* ret =  new btBoxShape(btVector3(btScalar(x), btScalar(y), btScalar(z)));
		btScalar mass(m);
		bool isDynamic = (m!= 0.f);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			ret->calculateLocalInertia(mass, localInertia);
		return (int)ret;
	}

	int WASM_EXP createPlane()
	{
		return 0;
	}

	int WASM_EXP createSphere(float r, float m)
	{
		return 0;
	}

	int WASM_EXP createCapsule(float r, float h, float m)
	{
		return 0;
	}

	void WASM_EXP deleteShape(int id)
	{
		btCollisionShape* pShape = (btCollisionShape*)id;
		delete pShape;
	}

	////// RigidBody //////
	btRigidBody::btRigidBodyConstructionInfo gRBI(0, nullptr, nullptr, btVector3(0,0,0));
	void WASM_EXP RBInfoSetMass(float mass)
	{
		gRBI.m_mass = mass;
	}

	void WASM_EXP RBInfoSetShape(int shape)
	{
		gRBI.m_collisionShape = (btCollisionShape*)shape;
	}

	void WASM_EXP RBInfoSetInertia(float x, float y, float z)
	{
		gRBI.m_localInertia.setX(x);
		gRBI.m_localInertia.setY(y);
		gRBI.m_localInertia.setZ(z);
	}
	
	int WASM_EXP createRigidBodyByRBInfo(int bodyid )
	{
		bodyMotionState* pMs = new bodyMotionState();
		gRBI.m_motionState = (btMotionState*)pMs;
		JSBtRigidBody* pRet = new JSBtRigidBody(gRBI);
		pMs->pBody = pRet = pRet;
		pRet->jsID = bodyid;
		return (int)pRet;
	}

	void WASM_EXP bodySetID(int body, int id) {
		btRigidBody* pBody = (btRigidBody*)body;
	}

	int WASM_EXP bodyGetPose(int body){
		JSBtRigidBody* pBody = (JSBtRigidBody*)body;
		return (int)&((bodyMotionState*)pBody->getMotionState())->m_graphicsWorldTrans; 
	}

	void WASM_EXP bodySetTransform(int trans) {
		btTransform* pTrans = (btTransform*)trans;
		//TODO 
	}

	void WASM_EXP bodySetG(float x, float y, float z)
	{
	}

	void WASM_EXP bodySetVel(float x, float y, float z)
	{

	}

	void WASM_EXP deleteRigidBody(int rb)
	{
		btRigidBody* pBody = (btRigidBody*)rb;
		bodyMotionState* pMS = (bodyMotionState*)pBody->getMotionState();
		if (pMS){
			delete pMS;
		}
		delete pBody;
	}

	btTransform gTmpTransform;
	int WASM_EXP getATmpTransorm() {
		return (int)&gTmpTransform;
	}

}
