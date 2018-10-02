#include <string>
#include <iostream>
#include <sstream>
#include "Team.h"
#include "Standing.h"
#include <msclr/marshal_cppstd.h>
#using <mscorlib.dll>
#using <System.Data.dll>
#using <System.dll>

using namespace System;
using namespace System::Data::OleDb;

ref class Database
{
private:
	static String^ query;
	static std::string* str;
	static std::string* error;
	static OleDbDataReader^ resultsReader;
	static OleDbConnection^ database;
	static OleDbCommand^ command;
public:
	static std::string getNextMatch(std::vector<Team>& teams)
	{
		try {
			database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
			database->Open();
			query = "SELECT TOP 1 * FROM matches WHERE hasPlayed=false ORDER BY matchNumber ASC";
			command = gcnew OleDbCommand(query, database);

			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			if (resultsReader->HasRows)
			{
				while (resultsReader->Read())
				{
					teams[0].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["red1"]->ToString()));
					teams[1].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["red2"]->ToString()));
					teams[2].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["red3"]->ToString()));
					teams[3].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["blue1"]->ToString()));
					teams[4].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["blue2"]->ToString()));
					teams[5].setNumber(msclr::interop::marshal_as<std::string>(resultsReader["blue3"]->ToString()));
					return msclr::interop::marshal_as<std::string>(resultsReader["matchNumber"]->ToString());
				}
			}
			else
				return "Done";
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}

		resultsReader->Close();
		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}

	static void getTeamMatches(std::vector<Team>& teams)
	{
		try {
			for (int i = 0; i < 6; i++)
			{
				database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
				if (database->State == System::Data::ConnectionState::Closed)
					database->Open();
				query = "SELECT TOP 1 matchNumber FROM " + msclr::interop::marshal_as<String^>(teams[i].getNumber()) + " WHERE hasPlayed=false ORDER BY matchNumber ASC";
				command = gcnew OleDbCommand(query, database);
				resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
				while (resultsReader->Read())
				{
					teams[i].setTeamMatch(msclr::interop::marshal_as<std::string>(resultsReader["matchNumber"]->ToString()));
				}
				//std::cout << "Match: " << teams[i].getTeamMatch() << std::endl;
				resultsReader->Close();
			}
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}

		resultsReader->Close();
		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}

	static void updateTeamMatch(Team teamObj)
	{
		try {
			database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			//std::cout << "opened\n";
			// SQL Query
			//		UPDATE [teamNumber] SET hasPlayed=true, autonGear='[Middle, Side, Miss, None, Unset]', teleopVault=[teleopVaultValue],
			//			teleopSwitch=[teleopSwitchValue], teleopScale=[teleopScaleValue], climb=[true, false] WHERE match=[teamMatch]
			query = "UPDATE " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " SET hasPlayed = true, autonSwitch = '";
			//std::cout << msclr::interop::marshal_as<std::string>(query->ToString()) << std::endl;
			query += msclr::interop::marshal_as<String^>(teamObj.getAutonSwitchStr()) + "', autonScale = '" + msclr::interop::marshal_as<String^>(teamObj.getAutonScaleStr());
			//std::cout << msclr::interop::marshal_as<std::string>(query->ToString()) << std::endl;
			query += "', teleopVault = '" + msclr::interop::marshal_as<String^>(teamObj.getTeleopVaultStr());
			query += "', teleopSwitch = " + msclr::interop::marshal_as<String^>(teamObj.getTeleopSwitchStr()) + ", teleopScale = " + msclr::interop::marshal_as<String^>(teamObj.getTeleopScaleStr());
			//std::cout << msclr::interop::marshal_as<std::string>(query->ToString()) << std::endl;
			query += ", climb ='" +msclr::interop::marshal_as<String^>(teamObj.getClimbStr());
			query += "' WHERE matchNumber = " + msclr::interop::marshal_as<String^>(teamObj.getTeamMatch());
			command = gcnew OleDbCommand(query, database);
			//std::cout << msclr::interop::marshal_as<std::string>(query->ToString()) << std::endl;
			command->ExecuteNonQuery();
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}
		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}

	static void updateTeamAverages(Team teamObj)
	{
		std::stringstream convert;
		std::string tempStr;
		double teleopVault = 0, teleopScale = 0, teleopSwitch = 0, tempDouble = 0;
		double gamesPlayedDouble = 0, autonScaleScoredDouble = 0, autonScaleAttemptedDouble = 0, autonSwitchScoredDouble = 0, autonSwitchAttemptedDouble = 0,
			climbAttemptedDouble = 0, climbScoredDouble = 0;

		try {
			database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
			
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT hasPlayed FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["hasPlayed"]->ToString() == "True")
					gamesPlayedDouble++;
			}

			resultsReader->Close();

			//std::cout << "First select\n";
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT teleopVault, teleopScale, teleopSwitch FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true";
			command = gcnew OleDbCommand(query, database);
			//std::cout << "team number\n";
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				convert.clear();
				tempStr = msclr::interop::marshal_as<std::string>(resultsReader["teleopVault"]->ToString());
				convert << tempStr;
				convert >> tempDouble;
				teleopVault += tempDouble;

				std::cout << teleopVault << " teleop vault\n";

				convert.clear();
				tempStr = msclr::interop::marshal_as<std::string>(resultsReader["teleopScale"]->ToString());
				convert << tempStr;
				convert >> tempDouble;
				teleopScale += tempDouble;

				std::cout << teleopScale << " teleop scale\n";

				convert.clear();
				tempStr = msclr::interop::marshal_as<std::string>(resultsReader["teleopSwitch"]->ToString());
				convert << tempStr;
				convert >> tempDouble;
				teleopSwitch += tempDouble;

				//std::cout << teleopSwitch << " teleop switch\n";

			}
			resultsReader->Close();

		//	std::cout << "Second select\n";

			//Auton Switch
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT autonSwitch FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (autonSwitch='Miss' OR autonSwitch='Success')";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if(resultsReader["autonSwitch"]->ToString() == "Success")
					autonSwitchScoredDouble++;
				//std::cout << "auton switch 1\n";
			}
			resultsReader->Close();
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT autonSwitch FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (autonSwitch='Null' OR autonSwitch='Miss' OR autonSwitch='Success')";
			command = gcnew OleDbCommand(query, database);
			//std::cout << "auton switch 2\n";
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["autonSwitch"]->ToString() == "Miss" || resultsReader["autonSwitch"]->ToString() == "Success")
					autonSwitchAttemptedDouble++;
			}
			resultsReader->Close();

			//Auton Scale
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT autonScale FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (autonScale='Miss' OR autonScale='Success')";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["autonScale"]->ToString() == "Success")
					autonScaleScoredDouble++;
			}
			resultsReader->Close();
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT autonScale FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (autonScale='Null' OR autonScale='Miss' OR autonScale='Success')";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["autonScale"]->ToString() == "Miss" || resultsReader["autonScale"]->ToString() == "Success")
					autonScaleAttemptedDouble++;
			}
			resultsReader->Close();

			//Climb
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT climb FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (climb='Miss' OR climb='Success')";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["climb"]->ToString() == "Success")
					climbScoredDouble++;
			}
			resultsReader->Close();
			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			query = "SELECT climb FROM " + msclr::interop::marshal_as<String^>(teamObj.getNumber()) + " WHERE hasPlayed=true AND (climb='Null' OR climb='Miss' OR climb='Success')";
			command = gcnew OleDbCommand(query, database);
			resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
			while (resultsReader->Read())
			{
				if (resultsReader["climb"]->ToString() == "Miss" || resultsReader["climb"]->ToString() == "Success")
					autonScaleAttemptedDouble++;
			}
			resultsReader->Close();

			//std::cout << "fourth select\nGames Played " << gamesPlayedDouble << std::endl;

			if (database->State == System::Data::ConnectionState::Closed)
				database->Open();
			// UPDATE totals SET avgteleopVault = [teleopVaultAvg], avgteleopScale = [teleopScaleAvg],
			//			avgteleopSwitch=[teleopSwitchAvg], perClimb=[climbPercentage], perAutonGearTotal=[autonGearTotalPercentage], 
			//			perAutonGearAttempted=[autonGearAttemptedPercentage] WHERE teamNumber=[teamNumber]
			query = "UPDATE totals SET avgTeleopVault=" + teleopVault/gamesPlayedDouble + ", avgTeleopScale=" + teleopScale/gamesPlayedDouble;
			//std::cout << "tele vault/scale" << std::endl;
			query += ", avgTeleopSwitch=" + teleopSwitch / gamesPlayedDouble + ", perClimb=" + climbScoredDouble / gamesPlayedDouble + ", perAutonSwitchTotal=" + autonSwitchScoredDouble / gamesPlayedDouble;
			query += ", perAutonSwitchAttempted=" ;
			if (autonSwitchAttemptedDouble == 0)
				query += "0";
			else
				query += autonSwitchScoredDouble / autonSwitchAttemptedDouble;
			query += ", perAutonScaleTotal=" + autonScaleScoredDouble / gamesPlayedDouble;
			query += ", perAutonScaleAttempted=";
			if (autonScaleAttemptedDouble == 0)
				query += "0";
			else
				query += autonScaleScoredDouble / autonScaleAttemptedDouble;
			query += " WHERE teamNumber=" + msclr::interop::marshal_as<String^>(teamObj.getNumber());

			//std::cout << msclr::interop::marshal_as<std::string>(query->ToString()) << std::endl;

			command = gcnew OleDbCommand(query, database);
			command->ExecuteNonQuery();

			//std::cout << "Updated\n";
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}
		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}

	static void closeMatch(std::string match)
	{
		try {
			database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
			database->Open();

			// SQL Query
			//		UPDATE [teamNumber] SET hasPlayed=true, autonGear='[Middle, Side, Miss, None, Unset]', teleopVault=[teleopVaultValue],
			//			teleopSwitch=[teleopSwitchValue], teleopScale=[teleopScaleValue], climb=[true, false] WHERE match=[teamMatch]
			query = "UPDATE matches SET hasPlayed=true WHERE matchNumber=" + msclr::interop::marshal_as<String^>(match);

			command = gcnew OleDbCommand(query, database);
			command->ExecuteNonQuery();
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}
		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}

	static void getStandingTeams(std::vector<Standing>& standings, std::string totalsColumns[])
	{
		try {
			int j = 0;
			for (int i = 0; i < 6; i++)
			{
				database = gcnew OleDbConnection("PROVIDER=Microsoft.Jet.OLEDB.4.0;Data Source=../scout.mdb");
				if(database->State == System::Data::ConnectionState::Closed)
					database->Open();
				query = "SELECT TOP 5 teamNumber, " + msclr::interop::marshal_as<String^>(totalsColumns[i]) + " FROM totals ORDER BY " + msclr::interop::marshal_as<String^>(totalsColumns[i]) + " DESC";
				command = gcnew OleDbCommand(query, database);
				resultsReader = command->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
				j = 0;
				while (resultsReader->Read())
				{
					standings[i].setTeamName(msclr::interop::marshal_as<std::string>(resultsReader["teamNumber"]->ToString()), j);
					standings[i].setTeamValue(msclr::interop::marshal_as<std::string>(resultsReader[msclr::interop::marshal_as<String^>(totalsColumns[i])]->ToString()), j);
					j++;
				}
				resultsReader->Close();
			}
		}
		catch (Exception^ ex)
		{
			*error = msclr::interop::marshal_as<std::string>(ex->ToString());
			std::cout << error << std::endl;
		}

		if (database->State == System::Data::ConnectionState::Open)
			database->Close();
	}
};