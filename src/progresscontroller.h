
// Generated by Tramway SDK Code generator

#include <framework/entity.h>
#include <framework/serialization.h>

#include <templates/macros.h>

using namespace tram;

class ProgressController : public Entity {
public:
	ProgressController();

	static void Init();
	
	TRAM_SDK_ENTITY_NAME("progress-controller")
	
	void UpdateParameters();
	void SetParameters();
	void Load();
	void Unload();
	void Serialize();
	void EventHandler(Event& evt);
	void MessageHandler(Message& msg);
	name_t GetType();
	
};
	