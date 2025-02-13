-- Nightmare Explorer v1.0 progress script

ResetProgressState()

found_key = false;				VariableDebug("found_key")
found_car = false;				VariableDebug("found_car")
found_ball = false;				VariableDebug("found_ball")
found_spray = false;			VariableDebug("found_spray")
found_scooter = false;			VariableDebug("found_scooter")
found_giblets = false;			VariableDebug("found_giblets")

searching_chocolate = false;	VariableDebug("searching_chocolate")
found_all_chocolate = false;	VariableDebug("found_all_chocolate")
gave_sandbox_guy_car = false;	VariableDebug("gave_sandbox_guy_car")
chocolate_found = 0;			VariableDebug("chocolate_found")

sandbox_guy_text = 0;			VariableDebug("sandbox_guy_text")

function ScriptProgress(t)
	print("Received progress trigger:", t)
	
	if t == "hello" then
		print("hello!")
		
		SetNPCDialog("hello!")
		SetNotification("hello!")
		
	elseif t == "gate" then
		SetNotification("The gate is locked.")
	
	elseif t == "car" then
		SetItemDisplay("Waow! You found car!", "item/car")
		found_car = true
	
	elseif t == "sandbox-guy" then
		local dialogs = {
			"I like sand.",
			"Sometimes I find chocolate in the sand.",
			"My mom dropped me on the head."
		}

		SetNPCDialog(dialogs[sandbox_guy_text % 3 + 1])
		
		SetNPCCallback("car")
		
		sandbox_guy_text = sandbox_guy_text + 1
	else 
		print("Unrecognized progress:", t)
	end
end