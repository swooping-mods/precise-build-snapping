#ifndef MOONLAB_PRECISION_SNAPPING_H
#define MOONLAB_PRECISION_SNAPPING_H

static int enabled=1,toggle_key,inside;
static float reach;
static PTR snap;

static int snap_finite(float value){
 return value>=-3.402823466e38f&&value<=3.402823466e38f;
}
static int snap_point_finite(DVec point){
 return snap_finite(point.x)&&snap_finite(point.y)&&snap_finite(point.z);
}
static void snap_fallback(Context *c,PTR selected,PTR hint,PTR target,DVec desired){
 *(int *)selected=0;*(PTR *)hint=0;dcopy(target,&desired,sizeof(desired));c->rax=0;
}
static int snap_ports(Context *c){
 /* Windows AMD64 CONTEXT XMM2 starts at byte 448. ConveyorPorts has
  * (this, Vector3 by reference, float radius in XMM2, List in R9).
  * Change only its scalar radius; vanilla still owns endpoint validation. */
 if(!enabled){
  PTR list=(PTR)c->r9;
  if(list){
   int ok=1;call(get_method(object_class(list),"Clear",0),list,0,&ok);
   if(!ok){bm.disabled=1;bevent("snap-endpoint-clear-failed");return 0;}
  }
  return 1;
 }
 float radius=0;dcopy(&radius,(unsigned char *)c+448,sizeof(radius));
 if(!(radius>=0))radius=0;
 else if(radius>reach)radius=reach;
 dcopy((unsigned char *)c+448,&radius,sizeof(radius));
 return 0;
}
static int snap_machine(Context *c){
 if(inside||!c->r8||!c->rsp)return 0;
 int kind=(int)c->rdx,turn=(int)c->r9;
 DVec desired={0},normal={0};dcopy(&desired,(PTR)c->r8,sizeof(desired));
 U64 *stack=(U64 *)c->rsp;
 PTR selected=(PTR)stack[5],hint=(PTR)stack[6],target=(PTR)stack[7],up=(PTR)stack[8];
 if(!selected||!hint||!target||!up)return 0;
 dcopy(&normal,up,sizeof(normal));
 if(!snap_point_finite(desired)||!snap_point_finite(normal))return 0;
 /* Preserve the original input before resetting possibly aliased out buffers.
  * Native failures and rejected plans must never publish partial snap state. */
 snap_fallback(c,selected,hint,target,desired);
 if(!enabled)return 1;

 int ok=1,native_selected=0;
 PTR native_hint=0;
 DVec native_target=desired;
 void *args[]={&kind,&desired,&turn,&native_selected,&native_hint,&native_target,&normal};
 inside=1;
 PTR native_plan=call(snap,dget(bm.world),args,&ok);
 DRoot plan=dhold(native_plan),message=dhold(native_hint);
 inside=0;
 PTR result=dget(plan),text=dget(message);
 int rooted=(!native_plan||result)&&(!native_hint||text);
 if(ok&&rooted&&snap_point_finite(native_target)){
  /* A successful null result may contain native validation diagnostics. */
  int accepted=!result;
  if(result){
   DVec position={0};PTR field=get_field(object_class(result),"position");
   if(field)field_value(result,field,&position);
   float distance2=moon_build_distance2((MoonBuildPoint){position.x,position.y,position.z},
                                      (MoonBuildPoint){desired.x,desired.y,desired.z});
   /* The positive comparison rejects NaN and overflow as well as long snaps. */
   accepted=field&&distance2<=reach*reach;
  }
  if(accepted){
   *(int *)selected=native_selected;*(PTR *)hint=text;
   dcopy(target,&native_target,sizeof(native_target));c->rax=(U64)result;
  }
 }
 ddrop(&message);ddrop(&plan);
 if(!ok||!rooted){bm.disabled=1;bevent(!ok?"snap-method-failed":"snap-output-root-failed");}
 return 1;
}
static int before(int hook,Context *c){
 if(hook==0){
  int state=bframe(c),toggle=baction("precise-build-snapping.toggle",toggle_key)|moon_shop_poll();
  if(state>0&&bm.eligible&&toggle){
   enabled=!enabled;bevent(enabled?"precision-snapping-enabled":"snapping-disabled");
   bstatus(enabled?"Moon Sweepers | Precision snapping ON | F9 toggle":"Moon Sweepers | Snapping OFF | F9 toggle");
  }
  return 0;
 }
 if(bm.disabled||!bm.host_ready||GetCurrentThreadId()!=bm.thread||
    !dget(bm.world)||(PTR)c->rcx!=dget(bm.world))return 0;
 if(hook==1)return snap_ports(c);
 if(hook==2)return snap_machine(c);
 return 0;
}
#endif
