#include "../../runtime/native/gameplay.h"
#include "building.h"
#include "config.h"
int _fltused=0;
static int enabled=1,toggle_key,inside;static float reach;static PTR snap;
static int before(int hook,Context *c){
 if(hook==0){int state=bframe(c),toggle=baction("precise-build-snapping.toggle",toggle_key)|moon_shop_poll();if(state>0&&bm.eligible&&toggle){enabled=!enabled;bevent(enabled?"precision-snapping-enabled":"snapping-disabled");bstatus(enabled?"Moon Sweepers | Precision snapping ON | F9 toggle":"Moon Sweepers | Snapping OFF | F9 toggle");}return 0;}
 if(bm.disabled||!bm.host_ready||GetCurrentThreadId()!=bm.thread||(PTR)c->rcx!=dget(bm.world))return 0;
 if(hook==1){
  /* Windows AMD64 CONTEXT XMM2 starts at byte 448. ConveyorPorts has
   * (this, Vector3 by reference, float radius in XMM2, List in R9).
   * Keep the engine's endpoint enumeration and all occupancy validation. */
  if(!enabled){int ok=1;PTR list=(PTR)c->r9;if(list)call(get_method(object_class(list),"Clear",0),list,0,&ok);return 1;}float radius=0;dcopy(&radius,(unsigned char *)c+448,4);if(radius>reach)dcopy((unsigned char *)c+448,&reach,4);return 0;
 }
 if(hook==2){
  if(inside)return 0;int ok=1,kind=(int)c->rdx,turn=(int)c->r9;
  DVec desired={0},normal={0};if(!c->r8)return 0;dcopy(&desired,(PTR)c->r8,sizeof(desired));
  U64 *stack=(U64 *)c->rsp;PTR selected=(PTR)stack[5],hint=(PTR)stack[6],target=(PTR)stack[7],up=(PTR)stack[8];if(!selected||!hint||!target||!up)return 0;dcopy(&normal,up,sizeof(normal));
  if(!enabled){*(int *)selected=0;*(PTR *)hint=0;dcopy(target,&desired,sizeof(desired));c->rax=0;return 1;}
  void *a[]={&kind,&desired,&turn,selected,hint,target,&normal};inside=1;DRoot plan=dhold(call(snap,dget(bm.world),a,&ok));inside=0;
  PTR result=dget(plan);if(result){DVec p={0};PTR field=get_field(object_class(result),"position");if(field)field_value(result,field,&p);float distance2=moon_build_distance2((MoonBuildPoint){p.x,p.y,p.z},(MoonBuildPoint){desired.x,desired.y,desired.z});
   if(!field||distance2>reach*reach){result=0;*(int *)selected=0;*(PTR *)hint=0;dcopy(target,&desired,sizeof(desired));}}
  c->rax=(U64)result;ddrop(&plan);if(!ok){bm.disabled=1;bevent("snap-method-failed");}return 1;
 }
 return 0;
}
__declspec(dllexport) int MoonLab_ModStart(PTR domain,PTR game){
 if(!bstart(domain,game,"precise-build-snapping"))return 0;cfg_start("MOONLAB_SNAPPING_CONFIG","MoonLab\\mods\\precise-build-snapping\\settings.ini");toggle_key=cfg("ToggleKey",0x78,1,255);reach=(float)cfg("ReachCentimetres",30,5,200)/100;enabled=cfg("Enabled",1,0,1);
 snap=dmethod("MoonCleaners","MoonWorld","SnapMachine","System.Int32;UnityEngine.Vector3;System.Int32;System.Int32&;System.String&;UnityEngine.Vector3&;UnityEngine.Vector3");
 if(!snap||!prepare_hook(method("MoonCleaners","MoonWalker","Update",0),100671733,0,game)||!prepare_hook(method("MoonCleaners","MoonWorld","ConveyorPorts",3),100664890,1,game)||!prepare_hook(snap,100665330,2,game))return 0;
 before_hook=before;if(!install_hooks())return 0;bevent("ready");return 1;
}
