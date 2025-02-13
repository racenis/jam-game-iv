
// Generated by Tramway SDK Code generator

#include "progresscontroller.h"
#include "progresscontroller.inl"

#include <framework/event.h>
#include <framework/message.h>

#include <framework/ui.h>
#include <render/render.h>

#include <components/render.h>

#include <framework/gui.h>
#include <extensions/menu/menu.h>

#include <framework/script.h>

#include <iostream>
#include <string>

static ProgressController* controller = nullptr;

std::vector<name_t> variable_debugs;

ProgressController::ProgressController() : Entity("progress") {
	frame.make(Event::FRAME, this);
	keypress.make(Event::KEYPRESS, this);
	
	auto comp = PoolProxy<RenderComponent>::New();
	comp->SetParent(this);
	comp->SetModel("dev/light");
	comp->SetLocation({0, 2, -5});
	comp->Init();
	
	Script::SetFunction("ResetProgressState", {}, [](valuearray_t array) -> value_t {
		return variable_debugs.clear(), true;
	});
	
	Script::SetFunction("VariableDebug", {TYPE_NAME}, [](valuearray_t array) -> value_t {
		return variable_debugs.push_back(array[0]), true;
	});
	
	Script::SetFunction("SetNPCDialog", {TYPE_STRING}, [](valuearray_t array) -> value_t {
		return controller->SetNPCDialog((const char*)array[0]), true;
	});
	
	Script::SetFunction("SetNotification", {TYPE_STRING}, [](valuearray_t array) -> value_t {
		return controller->SetNotification((const char*)array[0]), true;
	});
	
}

void ProgressController::Init() {
    controller = new ProgressController;
}

name_t ProgressController::GetType() {
    ENTITY_IMPLEMENTATION_GET_TYPE
}

void ProgressController::UpdateParameters() {
	if (!IsLoaded()) return;
}

void ProgressController::SetParameters() {
	if (!IsLoaded()) return;
	
	UpdateParameters();
}

void ProgressController::Load() {
	SetFlag(LOADED, true);
}

void ProgressController::Unload() {
	SetFlag(LOADED, false);
}

void ProgressController::Serialize() {

}

void ProgressController::SetNPCDialog(std::string dialog) {
	npc_text = dialog;
	npc_progress = dialog.length() + 120;
	npc_length = 0;
}

void ProgressController::SetNotification(std::string notif) {
	notif_text = notif;
	notif_progress = notif.length() + 120;
	notif_length = 0;
}

static std::string flag_helper(const char* text, bool value) {
	std::string result = text;
	while (result.length() < 21) result += " ";
	result += " [";
	if (value) result += "X";
	if (!value) result += "_";
	result += "]";
	return result;
}

static std::string flag_helper(const char* text, int value) {
	std::string result = text;
	while (result.length() < 21) result += " ";
	result += " [";
	result += std::to_string(value);
	result += "]";
	return result;
} 

void ProgressController::EventHandler(Event& evt) {
	switch (evt.type) {
		case Event::FRAME: {
			
			// draw the debug text in the top left corner
			std::string text = "Nightmare Exploration Simulator PRERELEASE";
			
			vec3 ploc = Entity::Find("player")->GetLocation();
			text += std::string("\nPosition: ") + std::to_string(ploc.x) + " "
			                                    + std::to_string(ploc.y) + " "
									            + std::to_string(ploc.z);
			
			for (auto variable : variable_debugs) {
				value_t value = Script::GetGlobal(variable);
				text += "\n";
				
				if (value.IsInt()) {
					text += flag_helper(variable, value.GetInt());
				} else {
					text += flag_helper(variable, (bool)value);
				}
			}
			
			Render::AddText(5, 0, text.c_str());
			
			
			// draw the NPC dialogs
			
			if (npc_progress) {
				std::string text = npc_text.substr(0, ++npc_length);
				npc_progress--;
				
				GUI::PushFrameRelative(GUI::FRAME_BOTTOM, 100);
					GUI::SetFont(Ext::Menu::FONT_PIXELART, GUI::TEXT);
					GUI::Text(text.c_str(), GUI::TEXT_CENTER);
					GUI::RestoreFont();
				GUI::PopFrame();
			}
			
			if (notif_progress) {
				std::string text = notif_text.substr(0, ++notif_length);
				notif_progress--;
				
				GUI::PushFrameRelative(GUI::FRAME_BOTTOM, 100);
					GUI::SetFont(Ext::Menu::FONT_PIXELART, GUI::TEXT);
					GUI::Text(text.c_str(), GUI::TEXT_CENTER);
					GUI::RestoreFont();
				GUI::PopFrame();
			}
			
		} break;
		case Event::KEYPRESS:
			;
			break;
		default:
			std::cout << "Progress unrecognized event: " << Event::GetName(evt.type) << std::endl; 
	}
}

void ProgressController::MessageHandler(Message& msg) {
	if (msg.type != Message::PING) return;
	
	name_t type = *msg.data_value;
	
	Script::CallFunction("ScriptProgress", {type});
}
	