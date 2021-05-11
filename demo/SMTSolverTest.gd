extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	var smtsolver = load("res://bin/smtsolver.gdns").new()
	smtsolver.add_constant("a", "Int")
	smtsolver.add_constant("b", "Int")
	smtsolver.add_constant("c", "Int")
	smtsolver.add_constant("d", "Real")
	smtsolver.add_constant("e", "Real")
	smtsolver.add_clause("> a (+ b 2)")
	smtsolver.add_clause("= a (+ (* 2 c) 10)")
	smtsolver.add_clause("<= (+ c b) 1000")
	smtsolver.add_clause(">= d e")
	print(smtsolver.is_sat())
	
	smtsolver = load("res://bin/smtsolver.gdns").new()
	smtsolver.add_constant("x", "Bool")
	smtsolver.add_constant("y", "Bool")
	var demorgan = "(!(x && y)) == (!x || !y)"
	smtsolver.add_clause(demorgan)
	if smtsolver.is_sat() == "sat":
		print("Demorgan is not valid")
	else:
		print("Demorgan is valid")
