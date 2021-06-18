#include <string>
#include <Windows.h>
#include "retcheck.h"
extern "C"
{
#include "Lua/lauxlib.h" // i told you one lua file.
}
using namespace std;
#define gdm __thiscall // ccv for first gdm. normally never changes.
#define gdmm __cdecl // ccv for second gdm

namespace r
{
	DWORD RL; // dont touch this is roblox.
	lua_State* L; // we prob dont even need this.
}

int ASLR(int x) 
{ 
	LPCSTR HD = 0; // LPCSTR.
	return(x - 0x400000 + reinterpret_cast<int>(GetModuleHandleA(HD))); // __stdcall.
	// 4194304 is 0x400000.
}

using def_getdatamodel = void(gdm*)(DWORD, DWORD);
using def_getdatamodel2 = DWORD(gdmm*)();

namespace arg // to execute more lua c shit, add more addressses.
{

	using rnewthread = DWORD(__cdecl*)(DWORD); // update the ccv.
	rnewthread addr_newthread = (rnewthread)(retcheckBypass(ASLR(0x01593530))); // need to update this.

	// usage: addr_pcall(r::RL, 1, 0, 0);
	using rpcall = int(__cdecl*)(DWORD lst, int nargs, int nresults, int errfunc); // update the ccv.
	rpcall addr_pcall = (rpcall)(retcheckBypass(ASLR(0x015937A0))); // need to update this.

	// usage: addr_getfield(RL, LUA_GLOBALSINDEX, "print");
	using rgetfield = int(__fastcall*)(DWORD lst, int idc, const char *t); // update the ccv.
	rgetfield addr_getfield = (rgetfield)(retcheckBypass(ASLR(0x01593030))); // need to update this.

	// usage: addr_pushstring(r::RL, "string is here.");
	using rpushstring = void(__cdecl*)(DWORD lst, const char*); // update the ccv.
	rpushstring addr_pushstring = (rpushstring)(retcheckBypass(ASLR(0x01593BC0))); // need to update this.

	// usage: not avaialbe?
	using rsetfield = void(__fastcall*)(DWORD v1, int v2, const char* v3); // update the ccv.
	rsetfield addr_setfield = (rsetfield)retcheckBypass(ASLR(0x01594690)); // need to update this.

	// usage: arg::addr_pushvalue(r::RL, -2);
	using rpushvalue = void(__cdecl*)(DWORD rL, DWORD idx); // update the ccv.
	rpushvalue addr_pushvalue = (rpushvalue)(retcheckBypass(ASLR(0x01593DC0))); // need to update this.

	using rpushnumber = double(__stdcall*)(DWORD rL, double idx); // update the ccv.
	rpushnumber addr_pushnumber = (rpushnumber)(retcheckBypass(ASLR(0x01593B60))); // need to update this.

	using rtolstring = char* (__stdcall*)(DWORD rL, int idx, size_t* size); // update the ccv.
	rtolstring addr_tolstring = (rtolstring)(retcheckBypass(ASLR(0x01594BA0))); // need to update this.

	void addr_getglobal(const char* k) // without typing arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "global");
	{
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, k);
	}
}

namespace cmds
{
	void Destroy(const char* part)
	{
		/*
		   lua example: game.Workspace.Model:Destroy()
		*/
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "game"); // we need "game".
		arg::addr_getfield(r::RL, -1, "Workspace"); // where the parent to destroy.
		arg::addr_getfield(r::RL, -1, part); // the models "name".
		arg::addr_getfield(r::RL, -1, "Destroy"); // do something to the "model".
		arg::addr_pushvalue(r::RL, -2); // -2.
		arg::addr_pcall(r::RL, 1, 0, 0); // this is pcall needed.
	}

	void Print(const char* msg)
	{
		/*
           lua example: local msg = "ok" print(msg)
        */ 
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "print"); // global is print.
		arg::addr_pushstring(r::RL, msg); // execute on the msg.
		arg::addr_pcall(r::RL, 1, 0, 0); // always do this.
	}

	void WalkSpeed(int speed)
	{
		// lua example: not available.
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "game"); // global game.
		arg::addr_getfield(r::RL, -1, "Players"); // inside the .
		arg::addr_getfield(r::RL, -1, "LocalPlayer"); // inside the .
		arg::addr_getfield(r::RL, -1, "Character"); // inside the .
		arg::addr_getfield(r::RL, -1, "Humanoid"); // inside the .
		arg::addr_pushnumber(r::RL, speed); // int this is walkspeed.
		arg::addr_setfield(r::RL, -2, "WalkSpeed"); // what we are modifying.
	}

	void Health(int h)
	{
		// lua example: not available.
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "game"); // global game.
		arg::addr_getfield(r::RL, -1, "Players"); // inside the .
		arg::addr_getfield(r::RL, -1, "LocalPlayer"); // inside the .
		arg::addr_getfield(r::RL, -1, "Character"); // inside the .
		arg::addr_getfield(r::RL, -1, "Humanoid"); // inside the .
		arg::addr_pushnumber(r::RL, h); // int this is walkspeed.
		arg::addr_setfield(r::RL, -2, "Health"); // what we are modifying.
	}

	void Gravity(int amt)
	{
		// lua example: local amt = 55 game.Workspace.Gravity = amt
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "game"); // global game.
		arg::addr_getfield(r::RL, -1, "Workspace"); // variant workspace.
		arg::addr_pushnumber(r::RL, amt); // amt this is gravity amt.
		arg::addr_setfield(r::RL, -2, "Gravity"); // what we are modifying.
	}

	void Kick(const char* msg) // ty skiehacker.
	{
		arg::addr_getglobal("game"); // decided to use getglobal.
		arg::addr_getfield(r::RL, -1, "Players"); // players.
		arg::addr_getfield(r::RL, -1, "LocalPlayer"); // player name aka local.
		arg::addr_getfield(r::RL, -1, "Kick"); // do some to them.
		arg::addr_pushvalue(r::RL, -2); // needed.
		arg::addr_pushstring(r::RL, msg); // execute on the msg.
		arg::addr_pcall(r::RL, 2, 0, 0); // this is pcall needed.
	}

	std::string GetUsername() { // needs to be in string for ex: std::string << GetUsername << std::endl;
		arg::addr_getfield(r::RL, LUA_GLOBALSINDEX, "game"); // this is a global.
		arg::addr_getfield(r::RL, -1, "Players"); // use in players.
		arg::addr_getfield(r::RL, -1, "LocalPlayer"); // localplayers.
		arg::addr_getfield(r::RL, -1, "Name"); // this is the name.
		return arg::addr_tolstring(r::RL, -1, NULL); // put the username into the tolstring.
	}
}

int ReturnDataModel() // start the gdm.
{
	DWORD DMPad[8]; // int.
	DWORD GetDataModelAdr = retcheckBypass(ASLR(0x00FF1C20)); // need to update this.
	DWORD GetDataModelAdr2 = retcheckBypass(ASLR(0x00FF1AD0)); // need to update this.
	reinterpret_cast<def_getdatamodel>(GetDataModelAdr)(reinterpret_cast<def_getdatamodel2>(GetDataModelAdr2)(), (DWORD)DMPad); // checks ccv.
	return DMPad[0] + 0xC; // defined as 12.
}

const char* GetClass(int self) // used for init.
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindFirstClass(int Instance, const char* Name) // use for init gdm.
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 44);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext + 204) ^ (ScriptContext + 204); } // update
DWORD RBX_SetIdentity(DWORD LS, int identity) { *(DWORD*)(*(DWORD*)(LS + 116) + 24) = identity; return 0; } // update