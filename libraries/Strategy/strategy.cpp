#include "strategy.h"

Move::Move(int left_motor_power, int right_motor_power, int time_ms)
{
    this->left_motor_power = left_motor_power;
    this->right_motor_power = right_motor_power;
    this->time_ms = time_ms;
    this->start_time_ms = 0;
    this->started = false;
    this->finished = false;
}


bool Move::update(MotorControl &left_motor, MotorControl &right_motor)
{
    int delta_time_ms = 0;
    int now_ms = millis();
    if (!this->started)
    {

        this->started = true;
        this->start_time_ms = now_ms;
        left_motor.setPower(this->left_motor_power);
        right_motor.setPower(this->right_motor_power);
        return false;
    }
    else
    {
        delta_time_ms = now_ms - this->start_time_ms;
        if (delta_time_ms >= this->time_ms)
        {
            this->finished = true;
            return true;
        }
        else
        {
            return false;
        }
    }
    char msg[] = "WRONG MOVE LOGIC";
    Serial.println(msg);
    return true;
}

InitialStrategy::InitialStrategy(std::list<Move> moves) : moves(moves), current_move(moves.front())
{
    this->moves.pop_front();
    this->strategy_finished = false;
}

bool InitialStrategy::update(MotorControl &left_motor, MotorControl &right_motor)
{
    if (this->strategy_finished)
    {
        return true;
    }

    bool current_move_finished;
    current_move_finished = this->current_move.update(left_motor, right_motor);

    if (current_move_finished)
    {
        // Serial.println(this->moves.empty());
        if (this->moves.empty())
        {
            this->strategy_finished = true;
            return true;
        }
        this->current_move = this->moves.front();
        this->moves.pop_front();

        this->current_move.update(left_motor, right_motor);
        return false;
    }
    else
    {
        return false;
    }

    char msg[] = "WRONG INITIAL STRATEGY LOGIC";
    Serial.println(msg);
    return true;
}

void AutoStrategy::updateMotors(Vision &vision, MotorControl &left_motor, MotorControl &right_motor)
{
    if (vision.enemy_position == EnemyPosition::FRONT)
    {
        left_motor.setPower(100);
        right_motor.setPower(100);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::LEFT)
    {
        left_motor.setPower(-30);
        right_motor.setPower(100);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::RIGHT)
    {
        left_motor.setPower(100);
        right_motor.setPower(-30);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::FULL_RIGHT)
    {
        left_motor.setPower(100);
        right_motor.setPower(-100);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::FULL_LEFT)
    {
        left_motor.setPower(-100);
        right_motor.setPower(100);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::SEARCH_LEFT)
    {
        left_motor.setPower(0);
        right_motor.setPower(100);
        return;
    }
    else if (vision.enemy_position == EnemyPosition::SEARCH_RIGHT)
    {
        left_motor.setPower(100);
        right_motor.setPower(0);
        return;
    }
}

InitialStrategy *get_selected_strategy(int pinA, int pinB, int pinC)
{
    bool selectA = digitalRead(pinA);
    bool selectB = digitalRead(pinB);
    bool selectC = digitalRead(pinC);

    int selected_number = int(selectA) + (int(selectB) << 1) + (int(selectC) << 2);
    
    Serial.print("\t");
    Serial.print("selectA: ");
    Serial.print(selectA);
    Serial.print("\t");
    Serial.print("selectB: ");
    Serial.print(selectB);
    Serial.print("\t");
    Serial.print("selectC: ");
    Serial.print(selectC);
    Serial.print("\t");
    Serial.print("selected_number: ");
    Serial.print(selected_number);
    Serial.print("\t");


    InitialStrategy *initial_strategy;
    std::list<Move> moves = {};
    switch (selected_number)
    {
    case 0: //Usar sensores 000
        break;
    case 1: //Vira pra esquerda, anda, gira pro centro do dohyo 001
        moves.push_back(Move(-50, 50, 75)); 
        moves.push_back(Move(100, 100, 250)); 
        moves.push_back(Move(100, -100, 75)); 
        break;
    case 2:  //Vira pra direita, anda, gira pro centro do dohyo 010
        moves.push_back(Move(-50, 50, 75)); 
        moves.push_back(Move(100, 100, 250)); 
        moves.push_back(Move(100, -100, 75));  
        break;
    case 3: //Curva eliptica esquerda 011
        moves.push_back(Move(-50, 50, 85)); 
        moves.push_back(Move(100, 80, 250)); 
        moves.push_back(Move(100, -100, 85));
        break;
    case 4: //Curva eliptica direita 100
        moves.push_back(Move(50, -50, 85)); 
        moves.push_back(Move(80, 100, 250)); 
        moves.push_back(Move(-100, 100, 85));
        break;
    case 5: //Curva agressiva esquerda 101
        moves.push_back(Move(50, -50, 85)); 
        moves.push_back(Move(100, 100, 250)); 
        moves.push_back(Move(-100, 100, 100));
        break;
    case 6: //Curva agressiva direita 110
        moves.push_back(Move(-50, 50, 85)); 
        moves.push_back(Move(100, 100, 250)); 
        moves.push_back(Move(100, -100, 100));
        break;
    case 7: //Desempate
        moves.push_back(Move(-100, 100, 100));
        break;
    default:
        break;
    }

    initial_strategy = new InitialStrategy(moves);

    return initial_strategy;
}

Move giraNoEixo(Direction direction, int motor_power, int time_ms) {
    if (direction == Direction::Left) {
        return Move(-motor_power, motor_power, time_ms);
    }
    else {
        return Move(motor_power, -motor_power, time_ms);
    }
}