// Board Platformer. A Board Game AI Developing Platform
// Copyright (C) 2017  Red-Portal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <utility>
#include <chrono>
#include <memory>
#include <stdint.h>

#include <board_platformer/game.hpp>
#include <board_platformer/types.hpp>

#include <grpc/grpc.h>

class board_platformer_comm;
class player_move;
class board_state;

namespace boost::process { class child; }

namespace board_platformer
{
    namespace ps = boost::process;
    namespace chrono = std::chrono;

    using comm = board_platformer_comm;
    using proto_player_move = player_move;
    using proto_board_state = board_state;
    using clock = chrono::steady_clock;
    using duration_t = chrono::duration<int64_t,std::milli>;

    class player
    {
    private:
        ps::child const _player_process;
        player_id_t const _player_id;

        std::unique_ptr<comm::Stub> _stub;

        proto_player_move
        send_message(proto_board_state const& board) const;

        std::vector<point_state>
        deserialize_moves(proto_player_move const& moves) const;

        proto_board_state
        serialize_board(game::game_board const& actual_board,
                        proto_board_state&& proto_board) const;

    public:
        explicit player(ps::child&& player_process,
                        adress_t const& adress,
                        player_id_t const& player_id);

        std::tuple<std::vector<point_state>, duration_t>
        play_turn(game::game_board const& board,
                  duration_t const& time_limit);
    };
}

#endif 
