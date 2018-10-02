#pragma once
#include <SFML/Graphics.hpp>

// Enumerator for the autonomous gear.
//enum teleopSwitchEnum { MIDDLE = 0, SIDE = 1, MISS = 2, NONE = 3, UNSET = 4 };
// Enumerator for climbing. (UNDEF = unset, but needed another word because UNSET is defined in teleopSwitchEnum.)

// Enumerator for the state of an individual team.
enum teamState { GAME, EDIT, WAIT };
// Enumerator for the selection in edit mode.
enum editSelection { A_SWITCH, A_SCALE, A_LINE, T_VAULT, T_SWITCH, T_SCALE, CLIMB, NO_EDIT };
enum autonSwitchEnum { UNSET = 0, FAIL = 1, SUCCESS = 2 };
enum autonScaleEnum { NO_TRY = 0, MISSED = 1, PLACED = 2 };
enum climbEnum { UNDEF = 0, NO = 1, YES = 2 };

class Team
{
private:
	// Team number value
	std::string number;
	// Joystick plugged in: true = yes; false = no
	bool isJoystick;
	// Autonomous gear value
	//teleopSwitchEnum teleopSwitch;
	// Match for team (1 through 12)
	std::string teamMatch;
	// Teleop vault value
	int teleopVault;
	// Teleop switch value
	int teleopSwitch;
	// Teleop scale value
	int teleopScale;
	// Climb value
	climbEnum climb;
	// Auton Line Crossed
	bool autonLine = false;
	// Auton Switch value
	autonSwitchEnum autonSwitch;
	// Auton Scale value
	autonScaleEnum autonScale;
	// State: GAME = match in progress; EDIT = change values; WAIT = submitted and waiting for next match
	teamState state;
	// Edit selection = auton/teleop balls/gears and climb
	editSelection editSelect;
	// Box to hold the joystick message
	sf::RectangleShape joystickBox;
	// Box for the team
	sf::RectangleShape box;
	// Box to hold the submit/edit message
	sf::RectangleShape submit;
	// Values have been put in the database: true = yes; false = no
	bool saved;

	// SFML text variables that allow for drawing text to the window
	// Team number
	sf::Text title;
	// Auton text
	sf::Text autonLabel;
	sf::Text autonSwitchLabel;
	sf::Text autonSwitchValue;
	sf::Text autonScaleLabel;
	sf::Text autonScaleValue;
	sf::Text autonLineLabel;
	sf::Text autonLineValue;
	// Teleop text
	sf::Text teleopLabel;
	sf::Text teleopVaultLabel;
	sf::Text teleopVaultValue;
	sf::Text teleopSwitchLabel;
	sf::Text teleopSwitchValue;
	sf::Text teleopScaleLabel;
	sf::Text teleopScaleValue;
	// Climb text
	sf::Text climbLabel;
	sf::Text climbValue;
	// Submit text
	sf::Text submitStatus;
	// Joystick message text
	sf::Text joystickText;
public:
	// Constructor
	// Start by resetting all the variables and setting the text for the text variables
	Team()
	{
		reset();
		joystickText.setString("Joystick Missing");
		autonLabel.setString("Auton");
		autonScaleLabel.setString("Scale");
		autonLineLabel.setString("Line");
		teleopSwitchLabel.setString("Switch");
		teleopVaultLabel.setString("Vault");
		teleopLabel.setString("Teleop");
		teleopScaleLabel.setString("Scale");
		climbLabel.setString("Climb");
		autonSwitchLabel.setString("Switch");
	} // END of constructor

	  // Setters
	void setNumber(std::string x) { number = x; };
	void setJoystick(bool x) { isJoystick = x; }
	void setTeamMatch(std::string x) { teamMatch = x; }
	void setteleopVault(int x) { teleopVault = x; }
	void setteleopSwitch(int x) { teleopSwitch = x; }
	void setteleopScale(int x) { teleopScale = x; }
	void setAutonSwitch(autonSwitchEnum x) { autonSwitch = x; }
	void setAutonScale(autonScaleEnum x) { autonScale = x; }
	void setAutonLine(bool x) { autonLine = x; }
	void setClimb(climbEnum x) { climb = x; }
	void setState(teamState x) { state = x; }
	void setEditSelection(editSelection x) { editSelect = x; }
	void setSaved(bool x) { saved = x; }

	// Getters for auton/teleop gears/balls and climb value text variables
	sf::Text& getTeleopSwitchValue() { return teleopSwitchValue; }
	sf::Text& getTeleopVaultValue() { return teleopVaultValue; }
	sf::Text& getTeleopScaleValue() { return teleopScaleValue; }
	sf::Text& getClimbValue() { return climbValue; }
	sf::Text& getAutonSwitchValue() { return autonSwitchValue; }
	sf::Text& getAutonScaleValue() { return autonScaleValue; }
	sf::Text& getAutonLineValue() { return autonLineValue; }

	// Getter for the box
	sf::RectangleShape& getBox() { return box; }


	// Getters for title (team number) and labels text variables
	sf::Text& getTitle() { return title; }
	sf::Text& getAutonLabel() { return autonLabel; }
	sf::Text& getTeleopSwitchLabel() { return teleopSwitchLabel; }
	sf::Text& getTeleopVaultLabel() { return teleopVaultLabel; }
	sf::Text& getTeleopLabel() { return teleopLabel; }
	sf::Text& getTeleopScaleLabel() { return teleopScaleLabel; }
	sf::Text& getClimbLabel() { return climbLabel; }
	sf::Text& getAutonSwitchLabel() { return autonSwitchLabel; }
	sf::Text& getAutonScaleLabel() { return autonScaleLabel; }
	sf::Text& getAutonLineLabel() { return autonLineLabel; }

	// Getters for joystick
	sf::RectangleShape& getJoystickBox() { return joystickBox; }
	sf::Text& getJoystickText() { return joystickText; }
	bool getJoystick() { return isJoystick; }

	// Getters for saving/submitting
	sf::RectangleShape& getSubmit() { return submit; }
	sf::Text& getSubmitStatus() { return submitStatus; }
	bool getSaved() { return saved; }

	// Getter for the team state
	teamState getState() { return state; }
	// Getter for the edit selection
	editSelection getEditSelection() { return editSelect; }
	// Getters for the team match
	std::string getTeamMatch() { return teamMatch; }

	// Getter for the team number
	std::string getNumber() { return number; }

	// Getters for the values for auton/teleop gears/balls and climb
	autonSwitchEnum getAutonSwitch() { return autonSwitch; }
	std::string getAutonSwitchStr()
	{
		switch (autonSwitch)
		{
		case autonSwitchEnum::UNSET: return "Null"; break;
		case autonSwitchEnum::FAIL: return "Miss"; break;
		case autonSwitchEnum::SUCCESS: return "Yes"; break;
		}
	}
	autonScaleEnum getAutonScale() { return autonScale; }
	std::string getAutonScaleStr()
	{
		switch (autonScale)
		{
		case autonScaleEnum::NO_TRY: return "Null"; break;
		case autonScaleEnum::MISSED: return "Miss"; break;
		case autonScaleEnum::PLACED: return "Yes"; break;
		}
	}
	int getTeleopVault() { return teleopVault; }
	std::string getTeleopVaultStr() { return std::to_string(teleopVault); }
	int getTeleopSwitch() { return teleopSwitch; }
	std::string getTeleopSwitchStr() { return std::to_string(teleopSwitch); }
	int getTeleopScale() { return teleopScale; }
	std::string getTeleopScaleStr() { return std::to_string(teleopScale); }
	bool getAutonLine() { return autonLine; }
	std::string getAutonLineStr()
	{
		switch (autonLine)
		{
		case true: return "True"; break;
		case false: return "False"; break;
			//case climbEnum::UNDEF: return "Unset"; break;
		}
	}
	climbEnum getClimb() { return climb; }
	std::string getClimbStr()
	{
		switch (climb)
		{
		case climbEnum::UNDEF: return "Null"; break;
		case climbEnum::NO: return "Miss"; break;
		case climbEnum::YES: return "Success"; break;
			//case climbEnum::UNDEF: return "Unset"; break;
		}
	}

	// Reset all the game variables
	void reset()
	{
		// Auton back to UNSET
		autonSwitch = autonSwitchEnum::UNSET;
		autonScale = autonScaleEnum::NO_TRY;
		autonLine = false;
		//teleop back to 0
		teleopVault = 0;
		teleopSwitch = 0;
		teleopScale = 0;
		// Climb back to UNDEF
		climb = climbEnum::UNDEF;
		// Edit selection to NO_EDIT
		editSelect = NO_EDIT;
	} // END of reset
};