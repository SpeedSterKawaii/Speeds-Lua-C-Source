#include "pch.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include "roblox.h"
#include <vector>
using namespace std;
#define haxname "Lua C Winning" // just a joke not an actual thing lol
int haxlevel = 6; // set the identity to this level.
#define SpeedMsgBox(msg) MessageBoxA(NULL, msg, "Speeds Lua C Source", NULL);
/*
* here is an open source lua c dll that totally uses 1 lua file.
* made by speedsterkawaii < credit me if ur gonna use for ur dog hax.
* 
* everything to update is in roblox.h please go there not here k thanks bye.
* edit june tenth of twerny tweerny one, added lua c commands check in roblox.h
*/

void exec(std::string str) // use str cuz we executing some stuff.
{
    // usage: LowGravity(), WalkSpeed(), KillMe().
    auto low_grav = "LowGravity()"; // check if it contains "lowgravity()"
    auto speed_me = "WalkSpeed()"; // check if it contains "walkspeed()"
    auto kill_me = "KillMe()"; // check if it contains "killme()"
    auto print = "Print()"; // check if it contains "print()"
    auto Kick = "Kick()"; // check if it contains "kick()"
    if (str.find(low_grav) != std::string::npos) { // if it does contain.
        cmds::Gravity(72); // do something "we are graviting".
    }
    else if (str.find(speed_me) != std::string::npos) { // if it does contain.
        cmds::WalkSpeed(152); // speed the humanoid to int.
    }
    else if (str.find(kill_me) != std::string::npos) { // if it does contain.
        cmds::Health(0); // kill myself for no reason lol.
    }
    else if (str.find(print) != std::string::npos) { // if it does contain.
        arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "print"); // global "print"
        arg::addr_pushstring(r::RL, "Printed from Lua C source."); // push the string to the global.
        arg::addr_pcall(r::RL, 1, 0, 0); // always do this.  
    }
    else if (str.find(Kick) != std::string::npos) { // if it does contain.
        cmds::Kick("You got kicked, sowwie."); // this is the msg ty skiehacker.
    }
    else
    {
        SpeedMsgBox("command doesn't exist.\nmake sure you type it right.\nerror 556: skill issue *cope*");
        // you prob typed it wrong or it doesn't exist yet :/
    }
}

void exec2(std::string str2) // disabled execution *ty ghandy*.
{
    string str = str2;
    if (str.find("WalkSpeed(1)") != std::string::npos) {
        // walkspeed with amt.
    }
    else if (str.find("WalkSpeed(2)") != std::string::npos) {
        // walkspeed with amt.
    }
    else if (str.find("WalkSpeed(3)") != std::string::npos) {
        // walkspeed with amt.
    }
    else if (str.find("WalkSpeed(4)") != std::string::npos) {
        // walkspeed with amt.
    }
}

DWORD WINAPI Pipe(PVOID lvpParameter)
{
    string WholeScript = ""; // script.
    HANDLE hPipe; // a handle.
    char buffer[999999]; // this is size.
    DWORD dwRead;
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\LuaCPipe"), // pipe name is changeable.
        PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        PIPE_WAIT,
        1, // int.
        999999, // int.
        999999, // int.
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                buffer[dwRead] = '\0';
                try {
                    try {
                        WholeScript = WholeScript + buffer;
                    }
                    catch (...) {
                    }
                }
                catch (std::exception e) {
                    // error.
                }
                catch (...) {
                    // error.
                }
            }
            exec(WholeScript); // do something to the pipe.
            WholeScript = "";
        }
        DisconnectNamedPipe(hPipe);
    }
}

void init() // this is the entry void
{
    Sleep(555); // cooldown for no reason.

    // show a message showing that dll injected.
    SpeedMsgBox("Lua C Open Source by SpeedSterKawaii.\nEnjoy this Open Source Exploit DLL.\nDon't skid too hard, or ill fuck you hard.\nClick OK to Start the Injection Process.\nCheck Console and See if It Warned Something.");

    // init via gdm address.
    DWORD DataModel = ReturnDataModel(); // return data model.
    DWORD ScriptContext = FindFirstClass(DataModel, "ScriptContext"); // sus.
    r::RL = arg::addr_newthread(RBX_LuaState(ScriptContext)); // update new thread addr.

    // set the identity to level.
    RBX_SetIdentity(r::RL, haxlevel); // haxlevel is the identity.
     
    // now make sure it works really quick.
    arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "warn"); // global "warn"
    arg::addr_pushstring(r::RL, "Lua C Source Works - Powered by Lua C Source."); // push the string to the global.
    arg::addr_pcall(r::RL, 1, 0, 0); // always do this.
    /* 
       in lua we can do warn"Lua C Source Works"
       but we are doing lua c so we do this above.
    */
    cmds::Destroy("Model"); // 'Model' is the part you are destroying.
    /*
       a lua example: game.Workspace.Model:Destroy()
       lua c would be the example you can view in roblox.h 
    */
    // rename the roblox title.
    HWND rbx = FindWindowA(NULL, "Roblox"); // get the window.
    SetWindowTextA(rbx, "Roblox - Modifed by LuaC Source by SpeedSterKawaii"); // rename to something.
    CreateThread(NULL, NULL, Pipe, NULL, NULL, NULL); // make the pipe to exec commands.
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: // or just type 1.
        init(); // when attached, we instantly go to the init void.
    case DLL_THREAD_ATTACH:
        // don't reattach into the prc again.
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break; // if roblox closes, we break the init.
    }
    return TRUE;
}

