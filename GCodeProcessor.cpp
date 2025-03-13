#include "GCodeProcessor.h"
#include "Logger.h"

/*
bool grab_cup(){
  capPicker->moveAbsolute(365);
}
*/

GCodeProcessor::GCodeProcessor(StepperMotor* u, StepperMotor* l, StepperMotor* c,
                   VibrMotor* vm, OptoSensor* ou, OptoSensor* ol, OptoSensor* oc,
                   LEDController* led)
    : upperDisk(u), lowerDisk(l), capPicker(c), vibrMotor(vm),
      optoUpper(ou), optoLower(ol), optoCap(oc), ledController(led), absoluteMode(true) {
}


bool GCodeProcessor::addChar(char c){
  return parser.AddCharToLine(c);
}

void GCodeProcessor::processLine() {
    LOG_DEBUG("Processing G-code: " + String(parser.line));
	LOG_DEBUG("Comment(s): " + String(parser.comments));
    
	parser.RemoveCommentSeparators();

    String commandID = "";
    if (parser.HasWord('I')) {
        commandID = String((int)parser.GetWordValue('I'));
    }

    // Обработка G-команд
    if (parser.HasWord('G')) {
        int code = (int)parser.GetWordValue('G');

        if (code == 90) { // G90: абсолютный режим
            absoluteMode = true;
            LOG_INFO("Mode: ABSOLUTE");
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 91) { // G91: относительный режим
            absoluteMode = false;
            LOG_INFO("Mode: RELATIVE");
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 2) { // G2: относительное перемещение (независимо от режима)
            LOG_INFO("G2: Relative move");
            if (parser.HasWord('U')) {
                float delta = parser.GetWordValue('U');
                upperDisk->moveRelative(delta);
            }
            if (parser.HasWord('L')) {
                float delta = parser.GetWordValue('L');
                lowerDisk->moveRelative(delta);
            }
            if (parser.HasWord('C')) {
                float delta = parser.GetWordValue('C');
                capPicker->moveRelative(delta);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 0 || code == 1) { // G0/G1: перемещение
            if (parser.HasWord('U')) {
                float val = parser.GetWordValue('U');
                if (absoluteMode)
                    upperDisk->moveAbsolute(val);
                else
                    upperDisk->moveRelative(val);
            }
            if (parser.HasWord('L')) {
                float val = parser.GetWordValue('L');
                if (absoluteMode)
                    lowerDisk->moveAbsolute(val);
                else
                    lowerDisk->moveRelative(val);
            }
            if (parser.HasWord('C')) {
                float val = parser.GetWordValue('C');
                if (absoluteMode)
                    capPicker->moveAbsolute(val);
                else
                    capPicker->moveRelative(val);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 92) { // G92: установка текущей позиции как нуля
            bool anyAxis = false;
            if (parser.HasWord('U')) { upperDisk->setOrigin(); anyAxis = true; }
            if (parser.HasWord('L')) { lowerDisk->setOrigin(); anyAxis = true; }
            if (parser.HasWord('C')) { capPicker->setOrigin(); anyAxis = true; }
            if (!anyAxis) {
                upperDisk->setOrigin();
                lowerDisk->setOrigin();
                capPicker->setOrigin();
                LOG_INFO("All axes origin set");
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 99) { // G99: печать текущих координат
            float posU = upperDisk->getPosition();
            float posL = lowerDisk->getPosition();
            float posC = capPicker->getPosition();
            Serial.println("[STATUS] Coordinates:");
            Serial.println("  Upper Disk (U): " + String(posU, 2) + "°");
            Serial.println("  Lower Disk (L): " + String(posL, 2) + "°");
            Serial.println("  Cap Picker (C): " + String(posC, 2) + "°");
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 117) { // G117: установка скорости (логирование)			//////////////////////////////////////////////////////////////////////////////////
            if (parser.HasWord('U')) {
                float speed = parser.GetWordValue('U');
                LOG_INFO("Upper Disk speed set to " + String(speed) + "°/s");
				        upperDisk->setSpeed(speed);
            }
            if (parser.HasWord('L')) {
                float speed = parser.GetWordValue('L');
                LOG_INFO("Lower Disk speed set to " + String(speed) + "°/s");
				        lowerDisk->setSpeed(speed);
            }
            if (parser.HasWord('C')) {
                float speed = parser.GetWordValue('C');
                LOG_INFO("Cap Picker speed set to " + String(speed) + "°/s");
				        capPicker->setSpeed(speed);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 118) { // G118: установка ускорения (логирование)			//////////////////////////////////////////////////////////////////////////////////
            if (parser.HasWord('U')) {
                float accel = parser.GetWordValue('U');
                LOG_INFO("Upper Disk acceleration set to " + String(accel) + "°/s²");
                upperDisk->setAcceleration(accel);
            }
            if (parser.HasWord('L')) {
                float accel = parser.GetWordValue('L');
                LOG_INFO("Lower Disk acceleration set to " + String(accel) + "°/s²");
                lowerDisk->setAcceleration(accel);
            }
            if (parser.HasWord('C')) {
                float accel = parser.GetWordValue('C');
                LOG_INFO("Cap Picker acceleration set to " + String(accel) + "°/s²");
                capPicker->setAcceleration(accel);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 28) { // G28: Homing
            if (parser.HasWord('U')) {
                while (!optoUpper->isTriggered()) {
                    upperDisk->moveRelative(-1);
                }
                upperDisk->setOrigin();
                LOG_INFO("Upper Disk homed");
            }
            if (parser.HasWord('L')) {
                while (!optoLower->isTriggered()) {
                    lowerDisk->moveRelative(-1);
                }
                lowerDisk->setOrigin();
                LOG_INFO("Lower Disk homed");
            }
            if (parser.HasWord('C')) {
                while (!optoCap->isTriggered()) {
                    capPicker->moveRelative(-2);
                }
                capPicker->setOrigin();
                LOG_INFO("Cap Picker homed");
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
    } // End G-commands

    // Обработка M-команд
    if (parser.HasWord('M')) {
        int code = (int)parser.GetWordValue('M');
        if (code == 1) {
            bool res = true;
            uint32_t starttime = millis();
            while (!optoCap->isTriggered()) {
              if (millis() - starttime >= 10000){
                res = false;
                break;
              }
                capPicker->moveRelative(-2);
            }
            Serial.println(res);
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 2) {
            if (parser.HasWord('S')) {
                uint32_t sleepTime = parser.GetWordValue('S');
                delay(sleepTime);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 3) {
            vibrMotor->turnOn();
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 4) {
            vibrMotor->turnOff();
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 5) {
            if (parser.HasWord('P')) {
                int power = (int)parser.GetWordValue('P');
                vibrMotor->setSpeed(power);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 8) {
            LOG_DEBUG("[OPTO] Upper sensor raw: " + String(optoUpper->getRawValue()));
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 9) {
            LOG_DEBUG("[OPTO] Upper sensor triggered: " + String(optoUpper->isTriggered()));
            Serial.println(optoUpper->isTriggered());
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 10) {
            LOG_DEBUG("[OPTO] Lower sensor raw: " + String(optoLower->getRawValue()));
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 11) {
            LOG_DEBUG("[OPTO] Lower sensor triggered: " + String(optoLower->isTriggered()));
            Serial.println(optoLower->isTriggered());
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 12) {
            LOG_DEBUG("[OPTO] Cap sensor raw: " + String(optoCap->getRawValue()));
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 13) {
            LOG_DEBUG("[OPTO] Cap sensor triggered: ");
            Serial.println(optoCap->isTriggered());
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 15) {
            Serial.println("[STATUS] System Status:");
            float posU = upperDisk->getPosition();
            float posL = lowerDisk->getPosition();
            float posC = capPicker->getPosition();
            Serial.println("  Upper Disk (U): " + String(posU, 2) + "°, " + (upperDisk->isMoving() ? "moving" : "stopped"));
            Serial.println("  Lower Disk (L): " + String(posL, 2) + "°, " + (lowerDisk->isMoving() ? "moving" : "stopped"));
            Serial.println("  Cap Picker (C): " + String(posC, 2) + "°, " + (capPicker->isMoving() ? "moving" : "stopped"));
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 30) {  // M30: установка порога датчиков (калибровка)
            if (parser.HasWord('U')) {
                int thr = (int)parser.GetWordValue('U');
                optoUpper->setThreshold(thr);
            }
            if (parser.HasWord('L')) {
                int thr = (int)parser.GetWordValue('L');
                optoLower->setThreshold(thr);
            }
            if (parser.HasWord('C')) {
                int thr = (int)parser.GetWordValue('C');
                optoCap->setThreshold(thr);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 31) {  // M31: инверсия логики датчиков
            if (parser.HasWord('U')) {
                int inv = (int)parser.GetWordValue('U');
                optoUpper->setInversion(inv != 0);
            }
            if (parser.HasWord('L')) {
                int inv = (int)parser.GetWordValue('L');
                optoLower->setInversion(inv != 0);
            }
            if (parser.HasWord('C')) {
                int inv = (int)parser.GetWordValue('C');
                optoCap->setInversion(inv != 0);
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 40) {  // M40: изменение уровня логирования
            if (parser.HasWord('L')) {
                int lvl = (int)parser.GetWordValue('L');
                if (lvl >= 0 && lvl <= 4) {
                    Logger::setLogLevel((LogLevel)lvl);
                }
            }
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 22) {  // M22: управление светодиодной лентой (цвет через HEX)
            if (parser.HasWord('H')) {
                uint32_t hexColor = parser.GetWordValue('H');
                ledController->setColorHex(hexColor);
                AckModule::sendAck(commandID, "OK");
            } else {
                AckModule::sendAck(commandID, "ERROR: Missing H parameter");
            }
            return;
        }
        if (code == 60) {  // M60: переход в спящий режим
            LOG_INFO("[SLEEP] Entering sleep mode...");
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 61) {  // M61: выход из спящего режима
            LOG_INFO("[SLEEP] Exiting sleep mode...");
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 62) {  // M62: освобождение двигателей
            upperDisk->release();
            lowerDisk->release();
            capPicker->release();
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 63) {  // M63: блокировка двигателей
            upperDisk->lock();
            lowerDisk->lock();
            capPicker->lock();
            AckModule::sendAck(commandID, "OK");
            return;
        }
        if (code == 110) {  // M110: фиксированное перемещение механизма захвата на 180°
            capPicker->moveRelative(180);
            AckModule::sendAck(commandID, "OK");
            return;
        }
    } // End M-commands
}
