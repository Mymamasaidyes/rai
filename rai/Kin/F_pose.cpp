#include "F_pose.h"
#include "TM_default.h"

void F_Pose::phi(arr& y, arr& J, const rai::KinematicWorld& C){
    NIY;
}

void F_Pose::phi(arr& y, arr& J, const WorldL& Ctuple){
#if 1
    TM_Default tmp(TMT_pos, a);
    tmp.order = order;
    tmp.type = TMT_pos;
    tmp.Feature::__phi(y, J, Ctuple);

    arr yq, Jq;
    tmp.type = TMT_quat;
    tmp.flipTargetSignOnNegScalarProduct=true;
    tmp.Feature::__phi(yq, (!!J?Jq:NoArr), Ctuple);

    y.append(yq);
    if(!!J) J.append(Jq);
#else //should be identical
    if(order==2){
      arr p0, p1, p2, J0, J1, J2;
      Ctuple(-3)->kinematicsPos(p0, J0, Ctuple(-3)->frames(a));  expandJacobian(J0, Ctuple, -3);
      Ctuple(-2)->kinematicsPos(p1, J1, Ctuple(-2)->frames(a));  expandJacobian(J1, Ctuple, -2);
      Ctuple(-1)->kinematicsPos(p2, J2, Ctuple(-1)->frames(a));  expandJacobian(J2, Ctuple, -1);

      y = p0 - 2.*p1 + p2;
      if(!!J) J = J0 - 2.*J1 + J2;

      arr q0, q1, q2; //, J0, J1, J2;
      Ctuple(-3)->kinematicsQuat(q0, J0, Ctuple(-3)->frames(a));  expandJacobian(J0, Ctuple, -3);
      Ctuple(-2)->kinematicsQuat(q1, J1, Ctuple(-2)->frames(a));  expandJacobian(J1, Ctuple, -2);
      Ctuple(-1)->kinematicsQuat(q2, J2, Ctuple(-1)->frames(a));  expandJacobian(J2, Ctuple, -1);

      if(scalarProduct(q0, q1)<-0.){ q0*=-1.; J0*=-1.; }
      if(scalarProduct(q2, q1)<-0.){ q2*=-1.; J2*=-1.; }

      arr yq = q0 - 2.*q1 + q2;
      arr Jq = J0 - 2.*J1 + J2;

      double tau = Ctuple(-2)->frames(0)->tau;
      tau=1.;
      if(tau){
        CHECK_GE(tau, 1e-10, "");
        yq /= tau*tau;
        if(!!J) Jq /= tau*tau;
      }

    }else if(order==1){
      arr p0, p1, J0, J1;
      Ctuple(-2)->kinematicsPos(p0, J0, Ctuple(-2)->frames(a));  expandJacobian(J0, Ctuple, -2);
      Ctuple(-1)->kinematicsPos(p1, J1, Ctuple(-1)->frames(a));  expandJacobian(J1, Ctuple, -1);

      y = p1 - p0;
      if(!!J) J = J1 - J0;

      arr q0, q1; //, J0, J1, J2;
      Ctuple(-2)->kinematicsQuat(q0, J0, Ctuple(-2)->frames(a));  expandJacobian(J0, Ctuple, -2);
      Ctuple(-1)->kinematicsQuat(q1, J1, Ctuple(-1)->frames(a));  expandJacobian(J1, Ctuple, -1);

      if(scalarProduct(q0, q1)<-0.){ q0*=-1.; J0*=-1.; }

      arr yq = q1 - q0;
      arr Jq = J1 - J0;

      y.append(yq);
      if(!!J) J.append(Jq);

      double tau = Ctuple(-2)->frames(0)->tau;
      if(tau){
        CHECK_GE(tau, 1e-10, "");
        y /= tau;
        if(!!J) J /= tau;
      }
    }
#endif
}

void F_PoseDiff::phi(arr& y, arr& J, const rai::KinematicWorld& C){
    NIY;
}

void F_PoseDiff::phi(arr& y, arr& J, const WorldL& Ctuple){
    arr yq, Jq;
    TM_Default tmp(TMT_posDiff, a, NoVector, b, NoVector);
    tmp.order = order;
    tmp.type = TMT_posDiff;
    tmp.Feature::__phi(y, J, Ctuple);
    tmp.type = TMT_quatDiff;
    tmp.flipTargetSignOnNegScalarProduct=true;
    tmp.Feature::__phi(yq, (!!J?Jq:NoArr), Ctuple);
    y.append(yq);
    if(!!J) J.append(Jq);
}

void F_PoseRel::phi(arr& y, arr& J, const rai::KinematicWorld& C){
    NIY;
}

void F_PoseRel::phi(arr& y, arr& J, const WorldL& Ctuple){
    arr yq, Jq;
    TM_Default tmp(TMT_pos, a, NoVector, b, NoVector);
    tmp.order = order;
    tmp.type = TMT_pos;
    tmp.Feature::__phi(y, J, Ctuple);
    tmp.type = TMT_quat;
    tmp.flipTargetSignOnNegScalarProduct=true;
    tmp.Feature::__phi(yq, (!!J?Jq:NoArr), Ctuple);
    y.append(yq);
    if(!!J) J.append(Jq);
}



