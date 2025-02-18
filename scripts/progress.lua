-- Nightmare Explorer v1.0 progress script

ResetProgressState()

found_key = false;				VariableDebug("found_key")
found_car = false;				VariableDebug("found_car")
found_ball = false;				VariableDebug("found_ball")
found_spray = false;			VariableDebug("found_spray")
found_toiletpaper = false;		VariableDebug("found_toiletpaper")
found_scooter = false;			VariableDebug("found_scooter")
found_giblets = false;			VariableDebug("found_giblets")

searching_chocolate = false;	VariableDebug("searching_chocolate")
found_all_chocolate = false;	VariableDebug("found_all_chocolate")
gave_sandbox_guy_car = false;	VariableDebug("gave_sandbox_guy_car")
chocolate_found = 0;			VariableDebug("chocolate_found")

sandbox_guy_text = 0;			VariableDebug("sandbox_guy_text")
witch_text = 0;					VariableDebug("witch_text")

killed_witch = false;			VariableDebug("killed_witch")
gave_away_ball = false;			VariableDebug("gave_away_ball")
opened_gate = false;			VariableDebug("opened_gate")

--found_key = true
--found_spray = true
--found_giblets = true

function Hint()
	if not searching_chocolate then
		SetNotification("Find the toilet creature.")
	elseif not found_car then
		SetNotification("Find the car.")
	elseif not gave_sandbox_guy_car then
		SetNotification("Give sandox guy the car.")
	elseif not found_all_chocolate then
		SetNotification("Find the sandbox chocolate.")
	elseif not found_spray then
		SetNotification("Go back to toilet creature.")
	elseif not killed_witch then
		SetNotification("Spray the witch.")
	elseif not found_ball then
		SetNotification("Find the ball.")
	elseif not gave_away_ball then
		SetNotification("Give the ball to the doorway creature.")
	elseif not found_giblets then
		SetNotification("Find the giblets.")
	elseif not found_scooter then
		SetNotification("Find the pool creature.")
	elseif not found_key then
		SetNotification("Find the key guy.")
	elseif not found_toiletpaper then
		SetNotification("Find the toilet paper.")
	elseif not opened_gate then
		SetNotification("Open the gate.")
	else
		SetNotification("That's it, you're done!")
	end
end

function ScriptProgress(t)
	print("Received progress trigger:", t)
	
	if t == "hello" then
		print("hello!")
		
		SetNPCDialog("hello!")
		SetNotification("hello!")
		
	elseif t == "end" then
		SetNotification("CONGRATULATION!")
		SetNotification("YOU ARE WINNER!")
		
	elseif t == "gate" then
		if not found_key then 
			SetNotification("The gate is locked.")
		elseif not opened_gate then
			SetNotification("The gate opens...")
			
			tram.event.AddListener(tram.event.TICK, function()
				local gate_pos = tram.entity.Find("gate"):GetLocation()
				gate_pos.x = gate_pos.x + 0.01
				gate_pos.z = gate_pos.z - 0.01
				tram.entity.Find("gate"):SetLocation(gate_pos)
			end)
			
			tram.message.Send({type = tram.message.KILL,
                               sender = 0,
                               receiver = tram.entity.Find("gate-barrier"):GetID(),
                               data = nil}, 0.0)
			
			opened_gate = true
		end
	elseif t == "giblets" then
		SetItemDisplay("Waow! You found giblets!", "item/giblets")
		found_giblets = true
	
	elseif t == "confused" then
		SetNotification("Tables, chairs, all confused.")
	
	elseif t == "ball" then
		SetItemDisplay("Waow! You found ball!", "item/ball")
		found_ball = true
		
	elseif t == "ball-guy" then
		if not found_ball then 
			SetNPCDialog("Can you get me the ball?")
			SetNPCDialog("I can't reach it!")
		elseif not gave_away_ball then
			SetNPCDialog("Thanks!")
			gave_away_ball = true
			
			tram.message.Send({type = tram.message.KILL,
                               sender = 0,
                               receiver = tram.entity.Find("ball-barrier"):GetID(),
                               data = nil}, 0.0)
							   
			local message = {type = tram.message.PING,
			                 sender = 0,
					         receiver = tram.entity.Find("ball-creature"):GetID(),
					         data = nil}
			tram.message.Send(message, 0.0)
		end
	elseif t == "car" then
		SetItemDisplay("Waow! You found car!", "item/car")
		found_car = true
		
	elseif t == "chocolate" then
		SetItemDisplay("Waow! You found a chocolate!", "item/chocolate")
		chocolate_found = chocolate_found + 1
		
		if chocolate_found >= 3 then found_all_chocolate = true end
	
	elseif t == "scooter" then
		SetItemDisplay("Waow! You found scooter!", "item/scooter")
		found_scooter = true
		
	elseif t == "spray" then
		SetItemDisplay("Waow! You found spray!", "item/spray")
		found_spray = true
	
	elseif t == "toiletpaper" then
		SetItemDisplay("Waow! You found toilet paper!", "item/toiletpaper")
		found_toiletpaper = true
		
	elseif t == "toilet" then
		if found_spray then
			SetNPCDialog("Go spray the witch!")
			
		elseif found_all_chocolate then
			SetNPCDialog("You have brought me 3 chocolates!")
			SetNPCDialog("You can now receive the reward!")
			chocolate_found = 0
			SetNPCCallback("spray")
			
			local message = {type = tram.message.PING,
			                 sender = 0,
					         receiver = tram.entity.Find("toilet-creature"):GetID(),
					         data = nil}
			tram.message.Send(message, 0.0)
			
		elseif searching_chocolate then
			SetNPCDialog("Bring chocolates and I will assist.")
			
		else
			SetNPCDialog("Find me 3 sandbox chocolates.")
			searching_chocolate = true
		end
		
	elseif t == "witch" then
		if found_spray and not killed_witch then
			SetNotification("You spray the witch.")
			SetNotification("The witch is dead.")

			local message = {type = tram.message.KILL,
			                 sender = 0,
					         receiver = tram.entity.Find("witch-barrier"):GetID(),
					         data = nil}
			tram.message.Send(message, 0.0)
			
			local message = {type = tram.message.PING,
			                 sender = 0,
					         receiver = tram.entity.Find("witch-creature"):GetID(),
					         data = nil}
			tram.message.Send(message, 0.0)
			
			print("SENDING!!!!")
			
			killed_witch = true
		elseif not killed_witch then
			local dialogs = {
				"I am busy!!!",
				"Go away!!!",
				"I am eating you!!!"
			}

			SetNPCDialog(dialogs[witch_text % 3 + 1])

			witch_text = witch_text + 1
		end
	elseif t == "sandbox-guy" then
		if found_car and not gave_sandbox_guy_car then
			SetNPCDialog("Waow! That is my car!")
			SetNPCDialog("Here: take this --")
			gave_sandbox_guy_car = true
			SetNPCCallback("chocolate")
		elseif searching_chocolate and not gave_sandbox_guy_car then
			SetNPCDialog("I lost my car!")
			SetNPCDialog("If you find it, I will trade my chocolate")
			SetNPCDialog("that I found while digging in the sand!")
		else
			local dialogs = {
				"I like sand.",
				"Sometimes I find chocolate in the sand.",
				"My mom dropped me on the head."
			}

			SetNPCDialog(dialogs[sandbox_guy_text % 3 + 1])

			sandbox_guy_text = sandbox_guy_text + 1
		end
	elseif t == "key" then
		SetItemDisplay("Waow! You found key!", "item/key")
		found_key = true
	
	elseif t == "key-guy" then
		if not found_scooter then
			SetNPCDialog("I found a key.")
			SetNPCDialog("I will trade you it for my scooter")
			SetNPCDialog("that the pool monster ate!")
		elseif not found_key then
			SetNPCDialog("Waow! My scooter!")
			SetNPCDialog("Okay, here's the key --")
			SetNPCCallback("key")
		end
	elseif t == "pool-monster" then
		if not found_giblets then
			SetNotification("Pool monster requests giblets.")
		elseif found_giblets and not found_scooter then
			SetNotification("Pool monster delivers scooter.")
			SetNotificationCallback("scooter")
		end	
	else 
		print("Unrecognized progress:", t)
	end
end