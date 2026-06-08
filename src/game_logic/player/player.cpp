#include "player.h"
#include "logger.h"
#include <random>
#include <string>

using std::to_string;

Player::Player() : x_(-1), y_(-1), z_(-1), escaped_(false), viewing_radius_(2), key_set_(6) {}

void Player::reset(int start_x, int start_y, int start_z, bool hard_mode) {
    x_ = start_x;
    y_ = start_y;
    z_ = start_z;
    escaped_ = false;

    if (hard_mode) {
        change_key_set();
    } else {
        set_default_key_set();
    }
}

void Player::set_position(int x, int y, int z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

bool Player::has_escaped() const {
    return escaped_;
}

void Player::set_escaped(bool escaped) {
    escaped_ = escaped;
}

int Player::get_x() const {
    return x_;
}

int Player::get_y() const {
    return y_;
}

int Player::get_z() const {
    return z_;
}

void Player::set_viewing_radius(int viewing_radius) {
    if (viewing_radius_ != viewing_radius) LOG("Current viewing_radius is " + std::to_string(viewing_radius_));
    viewing_radius_ = viewing_radius;
}

void Player::set_default_key_set() {
    key_set_ = {1, 11, 10, 12, 0, 2};
    LOG("Current key_set: {" + std::to_string(key_set_[0]) + ", "
        + std::to_string(key_set_[1]) + ", "
        + std::to_string(key_set_[2]) + ", "
        + std::to_string(key_set_[3]) + ", "
        + std::to_string(key_set_[4]) + ", "
        + std::to_string(key_set_[5]) + "}");
}

void Player::change_key_set()
{
    key_set_ = std::vector({1, 1, 1, 1, 1, 1});
    key_set_[0] = rand() % 26;
    key_set_[1] = key_set_[0];
    while (key_set_[1] == key_set_[0])
    {
        key_set_[1] = rand() % 26;
    }
    key_set_[2] = key_set_[1];
    while (key_set_[2] == key_set_[0] || key_set_[2] == key_set_[1])
    {
        key_set_[2] = rand() % 26;
    }
    key_set_[3] = key_set_[2];
    while (key_set_[3] == key_set_[0] || key_set_[3] == key_set_[1] || key_set_[3] == key_set_[2])
    {
        key_set_[3] = rand() % 26;
    }
    key_set_[4] = key_set_[3];
    while (key_set_[4] == key_set_[0] || key_set_[4] == key_set_[1] || key_set_[4] == key_set_[2] || key_set_[4] == key_set_[3])
    {
        key_set_[4] = rand() % 26;
    }
    key_set_[5] = key_set_[4];
    while (key_set_[5] == key_set_[0] || key_set_[5] == key_set_[1] || key_set_[5] == key_set_[2] || key_set_[5] == key_set_[3] || key_set_[5] == key_set_[4])
    {
        key_set_[5] = rand() % 26;
    }
    LOG("Current key_set: {" + std::to_string(key_set_[0]) + ", "
                             + std::to_string(key_set_[1]) + ", "
                             + std::to_string(key_set_[2]) + ", "
                             + std::to_string(key_set_[3]) + ", "
                             + std::to_string(key_set_[4]) + ", "
                             + std::to_string(key_set_[5]) + "}");
}

int Player::get_viewing_radius() const
{
    return viewing_radius_;
}

const std::vector<int>& Player::get_key_set() const
{
    return key_set_;
}

std::map<std::string, std::string> Player::get_state() const {
    std::map<std::string, std::string> state;
    state[TO_STR(x_)] = to_string(x_);
    state[TO_STR(y_)] = to_string(y_);
    state[TO_STR(z_)] = to_string(z_);
    state[TO_STR(escaped_)] = to_string(escaped_);
    state[TO_STR(viewing_radius_)] = to_string(viewing_radius_);
    for (int i = 0; i < 6; i++) state[std::string(TO_STR(key_set_)) + '[' + to_string(i) + ']'] = to_string(key_set_[i]);

    return state;
}

void Player::set_state(const std::map<std::string, std::string>& state) {
    x_ = std::stoi(state.at(TO_STR(x_)));
    y_ = std::stoi(state.at(TO_STR(y_)));
    z_ = std::stoi(state.at(TO_STR(z_)));
    escaped_ = (state.at(TO_STR(escaped_)) == "1");
    viewing_radius_ = std::stoi(state.at(TO_STR(viewing_radius_)));
    for (int i = 0; i < 6; ++i) key_set_[i] = std::stoi(state.at(std::string(TO_STR(key_set_)) + '[' + to_string(i) + ']'));
}
