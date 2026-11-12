#include "../../runtime/native/gameplay.h"
#include "building.h"
#include "config.h"
#include "snapping.h"
int _fltused=0;
__declspec(dllexport) int MoonLab_ModStart(PTR domain,PTR game){
 if(!bstart(domain,game,"precise-build-snapping"))return 0;cfg_start("MOONLAB_SNAPPING_CONFIG","MoonLab\\mods\\precise-build-snapping\\settings.ini");toggle_key=cfg("ToggleKey",0x78,1,255);reach=(float)cfg("ReachCentimetres",30,5,200)/100;enabled=cfg("Enabled",1,0,1);
 snap=dmethod("MoonCleaners","MoonWorld","SnapMachine","System.Int32;UnityEngine.Vector3;System.Int32;System.Int32&;System.String&;UnityEngine.Vector3&;UnityEngine.Vector3");
 if(!snap||!prepare_hook(method("MoonCleaners","MoonWalker","Update",0),100671733,0,game)||!prepare_hook(method("MoonCleaners","MoonWorld","ConveyorPorts",3),100664890,1,game)||!prepare_hook(snap,100665330,2,game))return 0;
 before_hook=before;if(!install_hooks())return 0;bevent("ready");return 1;
}
