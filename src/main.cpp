#include <framework/core.h>
#include <framework/logging.h>
#include <framework/ui.h>
#include <framework/gui.h>
#include <framework/async.h>
#include <framework/event.h>
#include <framework/message.h>
#include <framework/system.h>
#include <framework/worldcell.h>
#include <framework/language.h>
#include <framework/file.h>
#include <framework/path.h>
#include <framework/stats.h>
#include <framework/script.h>
#include <framework/loader.h>
#include <framework/settings.h>
#include <components/trigger.h>
#include <audio/audio.h>
#include <audio/sound.h>
#include <render/render.h>
#include <render/material.h>
#include <render/api.h>
#include <render/scene.h>
#include <physics/physics.h>
#include <physics/api.h>
#include <entities/player.h>
#include <entities/staticworldobject.h>
#include <entities/light.h>
#include <entities/crate.h>
#include <entities/marker.h>
#include <entities/trigger.h>
#include <entities/sound.h>
#include <entities/decoration.h>
#include <components/player.h>
#include <components/animation.h>
#include <components/controller.h>
#include <components/render.h>
#include <extensions/camera/camera.h>
#include <extensions/camera/firstperson.h>
#include <extensions/menu/menu.h>
#include <extensions/scripting/lua.h>
#include <extensions/kitchensink/kitchensink.h>
#include <extensions/kitchensink/entities.h>
#include <extensions/kitchensink/soundtable.h>

#include "creature.h"
#include "progresscontroller.h"

using namespace tram;
using namespace tram::UI;
using namespace tram::Render;
using namespace tram::Physics;
using namespace tram::Ext::Kitchensink;

void main_loop();

static bool moving = false;

Ext::Camera::FirstPersonCamera* camera = nullptr;
Player* player = nullptr;

int main(int argc, const char** argv) {
	Settings::Parse(argv, argc);
	
	Light::Register();
    Crate::Register();
    Sound::Register();
    Marker::Register();
    Decoration::Register();
    Trigger::Register();
    StaticWorldObject::Register();
    Ext::Kitchensink::Button::Register();
	
	// custom creature entity
	Creature::Register();
	
	Core::Init();
	UI::Init();
	Render::Init();
	Physics::Init();
#ifdef __EMSCRIPTEN__
	Async::Init(0);
#else
	Async::Init();
#endif
	Audio::Init();
	GUI::Init();

	Ext::Menu::Init();
	Ext::Camera::Init();
	Ext::Kitchensink::Init();
	
	Ext::Scripting::Lua::Init();
	Script::Init();

	Material::LoadMaterialInfo("material");
	Language::Load("en");
	
	Animation::Find("item-rotate")->Load();
	Animation::Find("pool-idle")->Load();
	Animation::Find("snake-get-out")->Load();
	Animation::Find("guard-die")->Load();
	Animation::Find("worm-open")->Load();
	
	// reusing sounds from central market catacombs
	Audio::Sound::Find("metalhit1")->Load();
	Audio::Sound::Find("metalhit2")->Load();
	Audio::Sound::Find("metalhit3")->Load();
	Audio::Sound::Find("metalhit4")->Load();
	Audio::Sound::Find("metalstep1")->Load();
	Audio::Sound::Find("metalstep2")->Load();
	Audio::Sound::Find("metalstep3")->Load();
	Audio::Sound::Find("woodhit1")->Load();
	Audio::Sound::Find("woodhit2")->Load();
	Audio::Sound::Find("woodhit3")->Load();
	Audio::Sound::Find("splash1")->Load();
	Audio::Sound::Find("splash2")->Load();
	Audio::Sound::Find("splash3")->Load();
	Audio::Sound::Find("concretehit1")->Load();
	Audio::Sound::Find("concretehit2")->Load();
	Audio::Sound::Find("concretehit3")->Load();
	Audio::Sound::Find("concretestep1")->Load();
	Audio::Sound::Find("concretestep2")->Load();
	Audio::Sound::Find("concretestep3")->Load();
	Audio::Sound::Find("slide1")->Load();
	Audio::Sound::Find("slide2")->Load();
	Audio::Sound::Find("metalsqueak1")->Load();
	Audio::Sound::Find("metalsqueak2")->Load();
	Audio::Sound::Find("metalsqueak3")->Load();
	Audio::Sound::Find("metalrattle1")->Load();
	Audio::Sound::Find("pickup1")->Load();
	Audio::Sound::Find("nailgun1")->Load();
	Audio::Sound::Find("nailgun2")->Load();
	Audio::Sound::Find("locked1")->Load();
	Audio::Sound::Find("snorkle1")->Load();
	Audio::Sound::Find("snorkle2")->Load();
	Audio::Sound::Find("snorkle3")->Load();
	
	SoundTable* metal_sounds = SoundTable::Find("metal");
	metal_sounds->AddSound(SOUND_HIT, "metalhit1");
	metal_sounds->AddSound(SOUND_HIT, "metalhit2");
	metal_sounds->AddSound(SOUND_WALK_ON, "metalstep1");
	metal_sounds->AddSound(SOUND_WALK_ON, "metalstep2");
	metal_sounds->AddSound(SOUND_WALK_ON, "metalstep3");
	
	SoundTable* wood_sounds = SoundTable::Find("wood");
	wood_sounds->AddSound(SOUND_HIT, "woodhit1");
	wood_sounds->AddSound(SOUND_HIT, "woodhit2");
	wood_sounds->AddSound(SOUND_HIT, "woodhit3");
	wood_sounds->AddSound(SOUND_WALK_ON, "woodhit1");
	wood_sounds->AddSound(SOUND_WALK_ON, "woodhit2");
	wood_sounds->AddSound(SOUND_WALK_ON, "woodhit3");
	
	SoundTable* water_sounds = SoundTable::Find("water");
	water_sounds->AddSound(SOUND_HIT, "splash1");
	water_sounds->AddSound(SOUND_HIT, "splash2");
	water_sounds->AddSound(SOUND_HIT, "splash3");
	water_sounds->AddSound(SOUND_WALK_ON, "splash1");
	water_sounds->AddSound(SOUND_WALK_ON, "splash2");
	water_sounds->AddSound(SOUND_WALK_ON, "splash3");
	
	SoundTable* concrete_sounds = SoundTable::Find("concrete");
	concrete_sounds->AddSound(SOUND_HIT, "concretehit1");
	concrete_sounds->AddSound(SOUND_HIT, "concretehit2");
	concrete_sounds->AddSound(SOUND_HIT, "concretehit2");
	concrete_sounds->AddSound(SOUND_WALK_ON, "concretestep1");
	concrete_sounds->AddSound(SOUND_WALK_ON, "concretestep2");
	concrete_sounds->AddSound(SOUND_WALK_ON, "concretestep3");
	
	// custom entity for receiving messages sent from triggers
	ProgressController::Init();
	
	Script::LoadScript("init");
	Script::LoadScript("progress");
	
	// this time we're using a single worldcell
	WorldCell* level = WorldCell::Make("level");
	level->LoadFromDisk();
	level->Load();
	
	// default player entity
	player = new Player;
	player->SetLocation(Entity::Find("player-start")->GetLocation());
	player->Load();
	
	// disabling mouselook by default
	Message msg;
	msg.sender = 0;
	msg.receiver = player->GetID();
	msg.type = Message::SET_FLAG_OFF;
	msg.data = Message::AllocateData<value_t>("mouselook");
	
	Message::Send(msg);
	
	// but we'll allow toggling mouselook if case someone dislikes it really bad
	UI::BindKeyboardKey(UI::KEY_M, [](){
		static bool mouselook = true;
		Message msg;
		msg.sender = 0;
		msg.receiver = player->GetID();
		msg.type = mouselook ? Message::SET_FLAG_ON : Message::SET_FLAG_OFF;
		msg.data = Message::AllocateData<value_t>("mouselook");
		
		Message::Send(msg);
		
		mouselook = !mouselook;
	});
	
	camera = new Ext::Camera::FirstPersonCamera;
	camera->SetMouselook(true);
	camera->SetRotateFollowing(true);
	camera->SetFollowingOffset({0, 0.4f, 0});
	camera->SetFollowing(player);
	camera->SetActive();
	
	camera->SetBobbingDistance(0.3f);
	camera->SetBobbingTilt(0.0f);
	camera->SetBobSpeed(0.1f);
	
	// play footstep sounds
	camera->SetBobbingCallback([](Ext::Camera::FirstPersonCamera*) {
		vec3 start = Render::GetViewPosition();
		vec3 direction = -DIRECTION_UP;
		vec3 forward = Render::GetViewRotation() * DIRECTION_FORWARD;
		vec3 side = Render::GetViewRotation() * DIRECTION_SIDE;
		
		start += 0.2f * side;
		
		auto result = Physics::Raycast(start, start + 2.0f * direction, ~(Physics::COLL_TRIGGER | Physics::COLL_PLAYER));
		
		if (result.collider) {
			auto res = Render::AABB::FindNearestFromRay(start, direction, -1);
			if (res.data && res.type == AABB::REFERENCE_RENDERCOMPONENT) {
				MaterialProperty prop = ((RenderComponent*)res.data)->GetModel()->GetMaterials()[res.triangle.material]->GetProperty();

				name_t mat_name;
				if (prop == PROPERTY_WOOD) {
					mat_name = "wood";
				} else if (prop == PROPERTY_SLIME) {
					mat_name = "water";
				} else if (prop == PROPERTY_CONCRETE) {
					mat_name = "concrete";
				} else {
					mat_name = "metal";
				}
				
				SoundTable::Find(mat_name)->PlaySound(SOUND_WALK_ON, 0.3f * forward + result.point - 0.2f * side, 1.0f);
			}
		}
	});
	
	// this checks whether a movement key is pressed
	Event::AddListener(Event::KEYPRESS, [](Event& evt) {
		switch (evt.subtype) {
			case UI::KEY_ACTION_FORWARD:
			case UI::KEY_ACTION_BACKWARD:
			case UI::KEY_ACTION_STRAFE_LEFT:
			case UI::KEY_ACTION_STRAFE_RIGHT:
				moving = true;
				break;
		}
	});
	
	// applies view bobbing if a movement key is pressed
	Event::AddListener(Event::TICK, [](Event& evt) {
		if (moving) {
			camera->SetBobbing(1.0f);
		} else {
			camera->SetBobbing(0.0f);
		}
		
		moving = false;
	});
	
	// add option to reload the main progress script
	/*UI::BindKeyboardKey(UI::KEY_KP_ENTER, [](){
		Script::LoadScript("progress");
	});*/
	
	#ifdef __EMSCRIPTEN__
		UI::SetWebMainLoop(main_loop);
	#else
		while (!UI::ShouldExit()) {
			main_loop();
		}

		Ext::Scripting::Lua::Uninit();
		
		Async::Yeet();
		Audio::Uninit();
		UI::Uninit();
	#endif
}

void main_loop() {
	Core::Update();
	UI::Update();
	Physics::Update();	

	GUI::Begin();
	
	Event::Dispatch();
	Message::Dispatch();
	
	Ext::Menu::Update();

	GUI::End();
	GUI::Update();
	
#ifdef __EMSCRIPTEN__
	Async::LoadResourcesFromDisk();
#endif
	Async::LoadResourcesFromMemory();
	Async::FinishResources();
	
	Loader::Update();
	
	AnimationComponent::Update();
	if (!Async::GetWaitingResources()) ControllerComponent::Update();
	
	Ext::Camera::Update();
	
	Render::Render();
	UI::EndFrame();
	
	Stats::Collate();
}