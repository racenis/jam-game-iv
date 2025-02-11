
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

#include <iostream>
#include <string>

static ProgressController* controller = nullptr;

ProgressController::ProgressController() : Entity("progress") {
	frame.make(Event::FRAME, this);
	keypress.make(Event::KEYPRESS, this);
	
	auto comp = PoolProxy<RenderComponent>::New();
	comp->SetParent(this);
	comp->SetModel("dev/light");
	comp->SetLocation({0, 2, -5});
	comp->Init();
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
			
			text += flag_helper("\nfound_key", found_key);
			text += flag_helper("\nfound_car", found_car);
			text += flag_helper("\nfound_ball", found_ball);
			text += flag_helper("\nfound_spray", found_spray);
			text += flag_helper("\nfound_scooter", found_scooter);
			text += flag_helper("\nfound_giblets", found_giblets);
			
			text += flag_helper("\nsearching_chocolate", searching_chocolate);
			text += flag_helper("\nfound_all_chocolate", found_all_chocolate);
			text += flag_helper("\ngave_sandbox_guy_car", gave_sandbox_guy_car);
			text += flag_helper("\nchocolate_found", chocolate_found);
			
			text += flag_helper("\nsandbox_guy_text", sandbox_guy_text);
			
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
	
	if (type == "hello") {
		std::cout << "hello!" << std::endl;
		
		SetNPCDialog("hello!");
	
	} else if (type == "gate") {
		SetNPCDialog("The gate is locked.");
	} else if (type == "sandbox-guy") {
		switch (sandbox_guy_text++ % 3) {
			case 0: SetNPCDialog("I like sand."); break;
			case 1: SetNPCDialog("Sometimes I find chocolate in the sand."); break;
			case 2: SetNPCDialog("My mom dropped me on the head."); break;
		}
	} else if (type == "gate-locked") {
		
	} else {
		std::cout << "Unrecognized progress: " << type << std::endl;
	}
	
	
	
	
	
	
	
	
	
	
	
	
}
	