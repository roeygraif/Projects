// #include "worldcup23a2.h"
// #include "assert.h"
// #include "wet2util.h"
// #include <iostream>
// int main(){
//     try{
//         world_cup_t worldCup;
//         StatusType sResult;




//         ///for get ith pointless ability, ignore
//         sResult = worldCup.get_ith_pointless_ability(5).status();
//         assert(sResult == StatusType::FAILURE);

//         ///ADDING 10 TEAMS
//         sResult = worldCup.add_team(10);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(2);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(3);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(9);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(4);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(8);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(6);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(7);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(1);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(5);
//         assert(sResult == StatusType::SUCCESS);

//         ///DUMMY TEAM
//         sResult = worldCup.add_team(11);
//         assert(sResult == StatusType::SUCCESS);


//         //////ALREADY EXISTS TEAM
//         sResult = worldCup.add_team(8);
//         assert(sResult == StatusType::FAILURE);

//         /////BAD INPUT TEAM
//         sResult = worldCup.add_team(0);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_team(-2);
//         assert(sResult == StatusType::INVALID_INPUT);



//         ////ADDING 100 PLAYERS
//         sResult = worldCup.add_player(1, 1, permutation_t::neutral(), 1, 1, 1, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(2, 2, permutation_t::neutral(), 2, 2, 2, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(3, 3, permutation_t::neutral(), 3, 3, 3, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(4, 4,permutation_t::neutral(), 4, 4, 4, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(5, 5,permutation_t::neutral(), 5, 5, 5, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(6, 6,permutation_t::neutral(), 6, 6, 6, false);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(7, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(8, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(9, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(10, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);



//         /////25 MEMBERS FOR TEAM 10
//         sResult = worldCup.add_player(11, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(12, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(13, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(14, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(15, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(16, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(17, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(18, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(19, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(20, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(21, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(22, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(23, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(24, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(25, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(26, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(27, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(28, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(29, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(30, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(31, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(32, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(33, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(34, 10, permutation_t::neutral(),10, 10, 10, true);
//         assert(sResult == StatusType::SUCCESS);



//         /////25 MEMBERS FOR TEAM 9
//         sResult = worldCup.add_player(35, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(36, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(37, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(38, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(39, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(40, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(41, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(42, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(43, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(44, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(45, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(46, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(47, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(48, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(49, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(50, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(51, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(52, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(53, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(54, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(55, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(56, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(57, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(58, 9, permutation_t::neutral(),9, 9, 9,true);
//         assert(sResult == StatusType::SUCCESS);

//         /////25 MEMBERS FOR TEAM 8
//         sResult = worldCup.add_player(59, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(60, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(61, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(62, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(63, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(64, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(65, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(66, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(67, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(68, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(69, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(70, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(71, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(72, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(73, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(74, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(75, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(76, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(77, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(78, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(79, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(80, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(81, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(82, 8,permutation_t::neutral(), 8, 8, 8, true);
//         assert(sResult == StatusType::SUCCESS);

//         //////19 MEMBERS FOR TEAM 7
//         sResult = worldCup.add_player(83, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(84, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(85, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(86, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(87, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(88, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(89, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(90, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(91, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(92, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(93, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(94, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(95, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(96, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(97, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(98, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(99, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(100, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);

//         ///DUMMY PLAYER
//         sResult = worldCup.add_player(101, 11, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::SUCCESS);

//         ///REMOVE DUMMY TEAM 11, SO DUMMY PLAYER 101 IS INACTIVE
//         sResult = worldCup.remove_team(11);
//         assert(sResult == StatusType::SUCCESS);

//         ///remove team checks
//         sResult = worldCup.remove_team(0);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.remove_team(-3);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.remove_team(33);
//         assert(sResult == StatusType::FAILURE);

//         ///TRY ADDING THIS TEAM AGAIN, SHOULD BE POSSIBLE
//         sResult = worldCup.add_team(11);
//         assert(sResult == StatusType::SUCCESS);



//         ///INVALID INPUT PLAYER
//         sResult = worldCup.add_player(0, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player(-33, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player(100, 0, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player(100, -22, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player(100, 7, permutation_t::neutral(),7, 7, -77, true);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player(100, 7, permutation_t::neutral(),7, 7, -7, true);
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///FAILURE PLAYER

//         sResult = worldCup.add_player(27, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.add_player(101, 7, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.add_player(100, 33, permutation_t::neutral(),7, 7, 7, true);
//         assert(sResult == StatusType::FAILURE);

//         ///remove dummy team again
//         sResult = worldCup.remove_team(11);
//         assert(sResult == StatusType::SUCCESS);



//         ///PLAY MATCH

//         ////FAILURE PLAY MATCH
//         sResult = (worldCup.play_match(27,3).status());
//         assert(sResult == StatusType::FAILURE);

//         sResult = worldCup.play_match(9,77).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.play_match(9,3).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.play_match(4,3).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.play_match(3,10).status();
//         assert(sResult == StatusType::FAILURE);


//         ///the deleted finally dummy
//         sResult = worldCup.play_match(11,3).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.play_match(5,11).status();
//         assert(sResult == StatusType::FAILURE);

//         ////INVALID INPUT PLAY MATCH
//         sResult = worldCup.play_match(0,3).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.play_match(-2,11).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.play_match(5,5).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.play_match(5,-2).status();
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///SUCCESS PLAY MATCH
//         assert(worldCup.play_match(9,10).ans() == 3);

//         assert(worldCup.play_match(8,7).ans() == 1);

//         ////FOR DRAW IN PLAY MATCH,IGNORE
//         sResult = worldCup.add_team(12);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_team(13);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(120, 12, permutation_t::neutral(),1, 1, 1, true);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player(121, 13, permutation_t::neutral(),1, 1, 1, true);
//         assert(sResult == StatusType::SUCCESS);

//         assert(worldCup.play_match(12,13).ans() == 0);


//         ///NUM PLAYED GAMES FOR PLAYER

//         ///NUM PLAYED GAMES FOR PLAYER FAILURE/INVALID INPUT
//         sResult = worldCup.num_played_games_for_player(999).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.num_played_games_for_player(0).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.num_played_games_for_player(-2).status();
//         assert(sResult == StatusType::INVALID_INPUT);

//         //////NUM PLAYED GAMES FOR PLAYER SUCCESS
//         assert(worldCup.num_played_games_for_player(5).ans() == 5);
//         assert(worldCup.num_played_games_for_player(7).ans() == 8);
//         assert(worldCup.num_played_games_for_player(8).ans() == 9);
//         assert(worldCup.num_played_games_for_player(9).ans() == 10);
//         assert(worldCup.num_played_games_for_player(10).ans() == 11);

//         ///WHAT ABOUT THE DUMMY?
//         assert(worldCup.num_played_games_for_player(101).ans() == 7);


//         ///ADD PLAYER CARDS

//         ///ADD PLAYER CARDS INVALID INPUT
//         sResult = worldCup.add_player_cards(0,5);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player_cards(-2,5);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player_cards(22,-3);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.add_player_cards(-2,-3);
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///ADD PLAYER CARDS FAILURE
//         sResult = worldCup.add_player_cards(99968,5);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.add_player_cards(7777,5);
//         assert(sResult == StatusType::FAILURE);

//         ///THE DUMMY
//         sResult = worldCup.add_player_cards(101,3);
//         assert(sResult == StatusType::FAILURE);

//         ///ADD PLAYER CARDS SUCCESS
//         sResult = worldCup.add_player_cards(73,5);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.add_player_cards(73,2);
//         assert(sResult == StatusType::SUCCESS);

//         ///CHECKING HIS CARDS
//         assert(worldCup.get_player_cards(73).ans() == 15);




//         ///GET PLAYER CARDS

//         ///GET PLAYER CARDS FAILURE/INVALID INPUT
//         sResult = worldCup.get_player_cards(999670).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.get_player_cards(0).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.get_player_cards(-77).status();
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///GET PLAYER CARDS SUCCESS
//         assert(worldCup.get_player_cards(73).ans() == 15);
//         assert(worldCup.get_player_cards(5).ans()== 5);
//         assert(worldCup.get_player_cards(10).ans() == 10);
//         assert(worldCup.get_player_cards(9).ans() == 9);

//         ///AND FOR THE DUMMY
//         assert(worldCup.get_player_cards(101).ans() == 7);



//         ////GET TEAM POINTS

//         ///GET TEAM POINTS FAILURE/INVALID_INPUT
//         sResult = worldCup.get_team_points(999670).status();
//         assert(sResult == StatusType::FAILURE);
//         ///THE DUMMY IS DELETED
//         sResult = worldCup.get_team_points(11).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.get_team_points(0).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.get_team_points(-777).status();
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///SUCCESS GET TEAM POINTS
//         assert(worldCup.get_team_points(3).ans() == 0);
//         assert(worldCup.get_team_points(7).ans() == 0);
//         assert(worldCup.get_team_points(8).ans() == 3);
//         assert(worldCup.get_team_points(9).ans() == 0);
//         assert(worldCup.get_team_points(10).ans() == 3);
//         ///AND WHAT ABOUT THE DUMMY DRAW TEAMS
//         assert(worldCup.get_team_points(12).ans() == 1);
//         assert(worldCup.get_team_points(13).ans() == 1);







//         ///GET ITH POINTLESS ABILITY

//         ///GET ITH FAILURE
//         sResult = worldCup.get_ith_pointless_ability(-3).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.get_ith_pointless_ability(888).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.get_ith_pointless_ability(13).status();
//         assert(sResult == StatusType::FAILURE);

//         ///GET ITH SUCCESS
//         assert(worldCup.get_ith_pointless_ability(11).ans() == 10);
//         assert(worldCup.get_ith_pointless_ability(10).ans() == 9);
//         assert(worldCup.get_ith_pointless_ability(9).ans() == 8);
//         assert(worldCup.get_ith_pointless_ability(8).ans() == 7);

//         ///WHAT ABOUT DRAW DUMMYS
//         assert(worldCup.get_ith_pointless_ability(0).ans() == 1);




//         ///GET PARTIAL SPIRIT

//         ///GET PARTIAL SPIRIT INVALID INPUT/FAILURE
//         sResult = worldCup.get_partial_spirit(90909).status();
//         assert(sResult == StatusType::FAILURE);
//         ///THE DUMMY PLAYER
//         sResult = worldCup.get_partial_spirit(101).status();
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.get_partial_spirit(0).status();
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.get_partial_spirit(-2).status();
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///GET PARTIAL SPIRIT "SUCCESS"
//         //////assert(worldCup.get_partial_spirit(73) == permutation_t::neutral());
//         //////assert(worldCup.get_partial_spirit(79) == permutation_t::neutral());



//         ///BUY TEAM

//         ///BUY TEAM FAILURE
//         sResult = worldCup.buy_team(555,3);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.buy_team(9,777);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.buy_team(222,223);
//         assert(sResult == StatusType::FAILURE);

//         ///BUY TEAM INVALID INPUT
//         sResult = worldCup.buy_team(-5,3);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.buy_team(-5,-5);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.buy_team(9,-30);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.buy_team(999,999);
//         assert(sResult == StatusType::INVALID_INPUT);
//         sResult = worldCup.buy_team(9,9);
//         assert(sResult == StatusType::INVALID_INPUT);

//         ///BUY TEAM SUCCESS
//         sResult = worldCup.buy_team(9,10);
//         assert(sResult == StatusType::SUCCESS);
//         sResult = worldCup.buy_team(8,7);
//         assert(sResult == StatusType::SUCCESS);


//         ///BUY TEAM CHECKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//         assert(worldCup.num_played_games_for_player(100).ans() ==8);
//         assert(worldCup.num_played_games_for_player(77).ans() == 9);
//         assert(worldCup.num_played_games_for_player(51).ans() == 10);
//         assert(worldCup.num_played_games_for_player(33).ans() == 11);

//         sResult = worldCup.add_player_cards(100,3);
//         assert(sResult == StatusType::SUCCESS);
//         assert(worldCup.get_player_cards(100).ans() == 10);

//         sResult = worldCup.add_player_cards(10,15);
//         assert(sResult == StatusType::SUCCESS);
//         assert(worldCup.get_player_cards(10).ans() == 25);

//         assert(worldCup.get_team_points(9).ans() == 3);
//         assert(worldCup.get_team_points(8).ans() == 3);

//         assert(worldCup.get_ith_pointless_ability(9).ans() == 9);

//         //////assert(worldCup.get_partial_spirit(100).ans() == permutation_t::neutral());


//         ///ANOTHER BUY TEAM CHECK
//         sResult = worldCup.remove_team(10);
//         assert(sResult == StatusType::FAILURE);
//         sResult = worldCup.remove_team(7);
//         assert(sResult == StatusType::FAILURE);

//         sResult = worldCup.buy_team(8,9);
//         assert(sResult == StatusType::SUCCESS);

//         sResult = worldCup.remove_team(9);
//         assert(sResult == StatusType::FAILURE);

//         assert(worldCup.get_team_points(8).ans() == 6);
//         assert(worldCup.get_ith_pointless_ability(8).ans() == 8);
//         assert(worldCup.get_ith_pointless_ability(7).ans() == 6);















//         /*
//                 assert(sResult == StatusType::SUCCESS);
//                 assert(worldCup.get_ith_pointless_ability(5).ans() == 8);

//                 assert(worldCup.num_played_games_for_player(1).ans() == 3);

//                 assert(worldCup.play_match(5,8).ans() == 1);
//                 assert(worldCup.num_played_games_for_player(1).ans() == 4);
//                 sResult = worldCup.add_player_cards(10, 219313);
//                 assert(sResult == StatusType::SUCCESS);
//                 assert(worldCup.get_player_cards(10).ans() == 219313);
//                 assert(        worldCup.get_team_points(5).ans() == 3);

//                 sResult = worldCup.add_player(18, 3,permutation_t::neutral(), 1, 1, 4, false);
//                 assert(sResult == StatusType::FAILURE);

//                 sResult = worldCup.add_player(21, 5, 1, 3, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(22, 7, 4, 0, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(23, 6, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(24, 2, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(25, 9, 2, 1, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(26, 9, 4, 0, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(27, 5, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(28, 9, 1, 2, 1, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(29, 7, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(30, 7, 3, 3, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(31, 7, 2, 2, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(32, 7, 4, 1, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(33, 6, 2, 2, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(34, 2, 4, 3, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(35, 8, 3, 3, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(36, 9, 3, 0, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(37, 1, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(38, 3, 4, 4, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(39, 5, 2, 1, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(40, 10, 4, 4, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(41, 10, 1, 1, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(42, 6, 3, 1, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(43, 7, 1, 0, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(44, 1, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(45, 1, 4, 3, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(46, 7, 1, 1, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(47, 9, 1, 2, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(48, 7, 1, 0, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(49, 4, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(50, 3, 0, 0, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(51, 2, 3, 4, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(52, 2, 2, 4, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(53, 5, 4, 4, 2, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(54, 5, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(55, 8, 3, 2, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(56, 7, 4, 4, 2, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(57, 10, 2, 1, 1, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(58, 3, 4, 4, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(59, 8, 2, 2, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(60, 8, 2, 3, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(61, 2, 1, 3, 2, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(62, 3, 2, 1, 3, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(63, 8, 2, 4, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(64, 1, 2, 2, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(65, 7, 1, 2, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(66, 2, 1, 1, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(67, 2, 2, 4, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(68, 4, 4, 1, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(69, 6, 3, 0, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(70, 7, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(71, 8, 3, 2, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(72, 6, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(73, 7, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(74, 10, 2, 0, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(75, 8, 3, 2, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(76, 1, 2, 1, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(77, 7, 3, 0, 2, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(78, 1, 2, 2, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(79, 10, 4, 0, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(80, 5, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(81, 2, 1, 0, 3, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(82, 9, 4, 0, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(83, 7, 10, 15, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(83, 7, 10, 15, 0, true);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.add_player(84, 10, 3, 0, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(85, 7, 4, 3, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(86, 6, 4, 2, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(87, 3, 4, 0, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(88, 10, 4, 1, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(89, 6, 2, 3, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(90, 9, 2, 10, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(91, 4, 0, 0, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(92, 3, 1, 0, 1, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(93, 7, 1, 1, 4, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(94, 6, 4, 3, 3, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(95, 4, 1, 3, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(96, 4, 3, 4, 0, false);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(97, 4, 2, 3, 4, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(98, 3, 0, 0, 0, true);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.add_player(99, 5, 3, 0, 1, false);
//                 assert(sResult == StatusType::SUCCESS);

//                 assert(worldCup.get_top_scorer(-1).ans() == 83);
//                 assert(worldCup.get_top_scorer(7).ans() == 83);
//                 worldCup.update_player_stats(22, 5, 40, 3);
//                 assert(worldCup.get_top_scorer(-1).ans() == 22);
//                 assert(worldCup.get_top_scorer(7).ans() == 22);
//                 assert(worldCup.get_top_scorer(9).ans() == 90);
//                 assert(worldCup.get_closest_player(98, 3).ans() == 91);
//                 assert(worldCup.get_closest_player(49, 4).ans() == 50);
//                 worldCup.unite_teams(1, 9, 19);
//                 sResult = worldCup.remove_team(1);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_team(9);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_team(19);
//                 assert(sResult == StatusType::FAILURE);
//                 int winner = worldCup.knockout_winner(2, 20).ans(); //Only 2, 6, 7 and 19 are competing
//                 worldCup.play_match(2, 19);
//                 worldCup.unite_teams(2, 7, 2);
//                 assert(worldCup.get_num_played_games(16).ans() == 5);

//                 // Remove team 6
//                 sResult = worldCup.remove_player(2);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(3);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(4);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(15);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(23);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(33);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(42);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(69);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(72);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(86);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(89);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::FAILURE);
//                 sResult = worldCup.remove_player(94);
//                 assert(sResult == StatusType::SUCCESS);
//                 sResult = worldCup.remove_team(6);
//                 assert(sResult == StatusType::SUCCESS);
//                 std::cout << "Test run successfully" << std::endl;
//                 return 0;*/
//     } catch(...){
//         return -1;
//     }
// }
