/*
    Niotso - Copyright (C) 2012 Fatbag <X-Fi6@phppoll.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define EXIT_SCENE() do { System::SceneFailed = true; delete this; return; } while(0)
#define SCENE_EXIT        0
#define SCENE_NEED_REDRAW 1
#define SCENE_NO_REDRAW   -1

class Scene {
    const float TickPeriod;
    float TimeDelta;
    virtual int Run(float TimeDelta) = 0;

  protected:
    Scene(float c) : TickPeriod(c), TimeDelta(0) {}

  public:
    int RunFor(float TimeDelta) {
        if(TickPeriod == 0){
            return Run(TimeDelta);
        }
        
        bool Redraw = false;
        this->TimeDelta += TimeDelta;
        while(this->TimeDelta >= 0){
            int result = Run(TickPeriod);
            if(result == System::SHUTDOWN)
                return System::SHUTDOWN;
            if(result > 0) Redraw = true;
            this->TimeDelta -= TickPeriod;
        }
        return (Redraw) ? 1 : -1;
    }
    
    virtual void Render() = 0;
};

class LoginScreen : public Scene {
  public:
    LoginScreen() : Scene(1.0f/15){
        Image_t * Image = File::ReadImageFile("eagames.bmp");
        if(!Image){
            const char * Message;
            switch(File::Error){
            case FERR_NOT_FOUND:
                Message = "%s does not exist.";
                break;
            case FERR_OPEN:
                Message = "%s could not be opened for reading.";
                break;
            case FERR_BLANK:
            case FERR_UNRECOGNIZED:
            case FERR_INVALIDDATA:
                Message = "%s is corrupt or invalid.";
                break;
            case FERR_MEMORY:
                Message = "Memory for %s could not be allocated.";
                break;
            default:
                Message = "%s could not be read.";
            }

            char Buffer[1024];
            sprintf(Buffer, Message, "eagames.bmp");
            MessageBox(Window::hWnd, Buffer, NULL, MB_OK | MB_ICONERROR);
            EXIT_SCENE();
        }
    }

  private:
    int Run(float){
        if(System::UserInput.CloseWindow){
            return SCENE_EXIT;
        }
        return SCENE_NEED_REDRAW;
    }

  public:
    void Render(){
    }
};