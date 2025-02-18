
// Generated by Tramway SDK Code generator

#include <framework/entity.h>
#include <framework/serialization.h>

#include <framework/event.h>

#include <components/render.h>
#include <components/animation.h>

#include <templates/macros.h>

#include <queue>

using namespace tram;

enum GameState {
	STATE_NORMAL,
	STATE_ITEM
};

class PrinterFormatter {
public:
	PrinterFormatter(std::string text, bool indefinite = false) {
		this->text = text;
		this->progress = (text.length() * (1.0f/60.0f)) + 2.0f;
		this->length = 0;

		if (indefinite) {
			this->progress = INFINITY;
		}
	}
	
	std::string GetText() {
		progress -= GetDeltaTime();
		return text.substr(0, ++length);	
	}
	
	bool IsEnd() {
		return progress < 0.0f;
	}
private:
	std::string text = "";
	float progress = 0.0f;
	int length = 0;
};

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
	
	GameState GetState() {
		return state;
	}
	
	void SetNPCDialog(std::string dialog); 
	void SetNPCCallback(name_t callback); 
	void SetNotification(std::string notif);
	void SetNotificationCallback(name_t callback); 	
	void SetItemDisplay(std::string text, name_t model);
	
private:
	GameState state = STATE_NORMAL;
	
	std::queue<PrinterFormatter> npc_queue;
	name_t npc_callback;
	
	std::queue<PrinterFormatter> notif_queue;
	name_t notif_callback;
	
	Component<RenderComponent> item_model;
	Component<AnimationComponent> item_animation;
	
	EventListener frame;
	EventListener keypress;
};
	