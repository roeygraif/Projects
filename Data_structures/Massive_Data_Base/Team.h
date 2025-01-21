
#ifndef WET2_TEAM_H
#define WET2_TEAM_H

#include "wet2util.h"
#include "player.h"

class Team
{
public:
    int teamId_m;
    int points_m;
    int goalKeepers_m;
    int num_of_players_m;
    int gamesPlayed_m;
    int newteam_gamesPlayed_m;
    int all_ability_m;
    bool valid_team_m;
    permutation_t team_spirit_m;
    PlayerNode* root_player_m;

public:
    
    bool checkIfLegalTeam()
    {
        if (goalKeepers_m > 0 && valid_team_m == true)
        {
            return true;
        }
        return false;
    }
    Team(int teamId = 0, int points = 0)
    {
        teamId_m = teamId;
        points_m = points;
        goalKeepers_m = 0;
        num_of_players_m = 0;
        gamesPlayed_m = 0;
        newteam_gamesPlayed_m = 0;
        all_ability_m = 0;
        valid_team_m = true;
        root_player_m = nullptr;
        team_spirit_m = permutation_t::neutral();
    }
    Team(const Team& team)
    {
        teamId_m = team.teamId_m;
        points_m = team.points_m;
        goalKeepers_m = team.goalKeepers_m;
        num_of_players_m = team.num_of_players_m;
        gamesPlayed_m = team.gamesPlayed_m;
        newteam_gamesPlayed_m = team.newteam_gamesPlayed_m;
        all_ability_m = team.all_ability_m;
        valid_team_m = team.valid_team_m;
        root_player_m = team.root_player_m;
        team_spirit_m = team.team_spirit_m;
    }
    void copyTeamData(const Team& team)
    {
        points_m = team.points_m;
        goalKeepers_m = team.goalKeepers_m;
        num_of_players_m = team.num_of_players_m;
        gamesPlayed_m = team.gamesPlayed_m;
        newteam_gamesPlayed_m = team.newteam_gamesPlayed_m;
        all_ability_m = team.all_ability_m;
        valid_team_m = team.valid_team_m;
        root_player_m = team.root_player_m;
        team_spirit_m = team.team_spirit_m;
    }
    ~Team()
    {
        this->root_player_m = nullptr;
    }

    bool operator<(const Team& team) const
    {
        if (teamId_m < team.teamId_m)
            return true;
        if (teamId_m == team.teamId_m)
            return (int)valid_team_m < (int)team.valid_team_m;
        return false;
    }
    bool operator>(const Team& team) const
    {
        if (teamId_m > team.teamId_m)
            return true;
        if (teamId_m == team.teamId_m)
            return (int)valid_team_m > (int)team.valid_team_m;
        return false;
    }
    bool operator==(const Team& team) const
    {
        return teamId_m == team.teamId_m && valid_team_m == team.valid_team_m;
    }
};


/**
* PointlessAbilityTeamWrapper:
* Wrapper class for Team class. This wrapper class is compareable by ability without including the team's points.
*/
class TeamPointlessAbility
{
public:
    int teamId_m;
    int teamAbility_m;

public:
    int getTeamId()const 
    {
        return teamId_m;
    }
    int getTeamAbility()const
    {
        return teamAbility_m;
    }
    TeamPointlessAbility(int teamId = 0, int ability = 0)
    {
        teamId_m = teamId;
        teamAbility_m = ability;
    }
    TeamPointlessAbility(const TeamPointlessAbility& pointlessTeam)
    {
        teamId_m = pointlessTeam.getTeamId();
        teamAbility_m = pointlessTeam.getTeamAbility();
    }
    TeamPointlessAbility(const Team& team)
    {
        teamId_m = team.teamId_m;
        teamAbility_m = team.all_ability_m;
    }
    ~TeamPointlessAbility() = default;

    bool operator<(const TeamPointlessAbility& teamAbility) const
    {
        if (teamAbility_m  == teamAbility.getTeamAbility())
        {
            return teamId_m < teamAbility.getTeamId();
        }
        return teamAbility_m < teamAbility.getTeamAbility();
    }
    bool operator>(const TeamPointlessAbility& teamAbility) const
    {
        if (teamAbility_m == teamAbility.getTeamAbility())
        {
            return teamId_m > teamAbility.getTeamId();
        }
        return teamAbility_m > teamAbility.getTeamAbility();
    }
    bool operator==(const TeamPointlessAbility& teamAbility) const
    {
        return (teamAbility_m == teamAbility.getTeamAbility() && teamId_m == teamAbility.getTeamId());
    }
};

#endif // WET2_TEAM_H