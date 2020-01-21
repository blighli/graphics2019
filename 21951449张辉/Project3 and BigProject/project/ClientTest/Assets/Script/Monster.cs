using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[AddComponentMenu("Game/Monster")]
public class Monster : MonoBehaviour
{
    public Transform m_transform;

    // 角色控制器组件
    CharacterController ch;

    // 动画组件
    Animator ani;

    // 角色移动速度
    float movSpeed = 3.0f;
    // 角色跑步加速系数
    float runSpeed_ratio = 1.5f;

    // 摄像机Transform
    Transform camTransform;


    // 生命值
    public int max_life = 10;
    public int now_life;
    //攻击属性

    float attack1_distance=3f;
    float attack2_distance=3f;
    int attack1_damage=1;
    int attack2_damage=2;

    //是否收到伤害,死亡
    bool isgethit=false,isdie=false;
    //伤害判定
    bool isdamage=false;

    //输入变量
    bool input_a,input_d,input_s,input_w,input_q,input_e,input_shift;
    //定义3个值控制移动
    float xm = 0, ym = 0, zm = 0;
    //水平移动基数
    float horizontal=0;
 
    //动画信息
    AnimatorStateInfo stateInfo;
     // 计时器
    float timer ;

    void Start()
    {
        m_transform = this.transform;
        // 获取角色控制器组件
        ch = this.GetComponent<CharacterController>();
        // 获得动画播放器
        ani = this.GetComponent<Animator>();

        camTransform =  GameObject.FindGameObjectWithTag ("MainCamera").transform;
        // camTransform =  Camera.main.transform;

        now_life=max_life;

    }

    void Update()
    {
        UpdateValue();
       
        UpdatePosition();

        UpdateAnimation();
    }
    //更新各个值
    void UpdateValue() 
    {
        stateInfo = ani.GetCurrentAnimatorStateInfo(0);

        //输入
        input_a=false;
        input_d=false;
        input_w=false;
        input_s=false;
        input_q=false;
        input_e=false;
        input_shift=false;


        xm = 0;
        ym = 0;
        zm = 0;

        if (Input.GetKey(KeyCode.A))
        {
            input_a = true;
        }
        if (Input.GetKey(KeyCode.D))
        {
            input_d = true;
        }
        if (Input.GetKey(KeyCode.W))
        {
            input_w= true;
        }
        if (Input.GetKey(KeyCode.S))
        {
            input_s = true;
        }
        if (Input.GetKey(KeyCode.Q))
        {
            input_q = true;
        }
        if (Input.GetKey(KeyCode.E))
        {
            input_e = true;
        }

        if ((Input.GetKey("left shift") || Input.GetKey("right shift")) )
        {
            input_shift = true;
        }

    }
    //更新角色位置,方向
    void UpdatePosition()
    {
        //前后左右(优先左上)
        if(input_a)
        {
            xm=-1;
            horizontal=1;
        }
        else if(input_d)
        {
            xm=1;
            horizontal=1;
        }
        if(input_w)
        {
            zm=1;
            horizontal=1;
        }
        else if(input_s)
        {
            zm=-1;
            horizontal=1;
        }

        Vector3 camrot = camTransform.eulerAngles;
     
        //方法二（有转向动作）
        Vector3 oriV=new Vector3(xm,0,zm);
        if(xm != 0 || zm != 0){
            // 计算出新方向
            //Quaternion.AngleAxis(camrot.y,Vector3.up)*oriV是oriV绕着y轴旋转相机的y轴偏角
             Vector3 newDir = Vector3.RotateTowards(m_transform.forward, Quaternion.AngleAxis(camrot.y,Vector3.up)*oriV, 20f * Time.deltaTime, 0.0f);
            // 旋转至新方向
            m_transform.rotation = Quaternion.LookRotation(newDir);
        }

        if(stateInfo.fullPathHash != Animator.StringToHash("Base Layer.Walk") && stateInfo.fullPathHash != Animator.StringToHash("Base Layer.Run"))
        {
           horizontal=0;
        }
        if(input_shift)
        {
            //水平方向
            ch.Move(horizontal*m_transform.forward*Time.deltaTime*movSpeed*runSpeed_ratio);
            //垂直方向
            ch.Move(transform.TransformDirection(new Vector3(0, ym, 0)));
        }
        else{
            //水平方向
            ch.Move(horizontal*m_transform.forward*Time.deltaTime*movSpeed);
            //垂直方向
            ch.Move(transform.TransformDirection(new Vector3(0, ym, 0)));
        }
    }
    //更新动画
    void UpdateAnimation()
    {
        // 如果处于待机
        if (stateInfo.fullPathHash == Animator.StringToHash("Base Layer.Idle") && !ani.IsInTransition(0))
        {
            ani.SetBool("idle", false);
            //受击,攻击1,攻击2，跑步，走路
            if(isgethit)
            {
                ani.SetBool("gethit",true);
            }
            else if(input_q)
            {
                ani.SetBool("attack1", true);
                isdamage=false;
            }
            else if(input_e)
            {
                ani.SetBool("attack2", true);
                isdamage=false;
            }
            else if((input_a||input_d||input_w||input_s)&&input_shift){
                ani.SetBool("run",true);
            }
            else if((input_a||input_d||input_w||input_s)&&!input_shift){
                ani.SetBool("walk",true);
            }
        }
        // 如果处于走路
        if (stateInfo.fullPathHash == Animator.StringToHash("Base Layer.Walk") && !ani.IsInTransition(0))
        {
            ani.SetBool("walk", false);
            //受击,攻击1,攻击2,跑步,静止
            if(isgethit)
            {
                ani.SetBool("gethit",true);
            }
            else if(input_q)
            {
                ani.SetBool("attack1", true);
                isdamage=false;
            }
            else if(input_e)
            {
                ani.SetBool("attack2", true);
                isdamage=false;
            }
            else if((input_a||input_d||input_w||input_s)&&input_shift){
                ani.SetBool("run",true);
            }
            else if(!(input_a||input_d||input_w||input_s)){
                ani.SetBool("idle",true);
            }
        }
        // 如果处于跑步
        if (stateInfo.fullPathHash == Animator.StringToHash("Base Layer.Run") && !ani.IsInTransition(0))
        {
            ani.SetBool("run", false);
            //受击,攻击1,攻击2,跑步,走路,静止
            if(isgethit)
            {
                ani.SetBool("gethit",true);
            }
            else if(input_q)
            {
                ani.SetBool("attack1", true);
                isdamage=false;
            }
            else if(input_e)
            {
                ani.SetBool("attack2", true);
                isdamage=false;
            }
            else if((input_a||input_d||input_w||input_s)&&input_shift){
                ani.SetBool("run",true);
            }
            else if((input_a||input_d||input_w||input_s)){
                ani.SetBool("walk",true);
            }
            else if(!(input_a||input_d||input_w||input_s)){
                ani.SetBool("idle",true);
            }
        }
        // 如果处于攻击1
        if (stateInfo.fullPathHash == Animator.StringToHash("Base Layer.Attack1") && !ani.IsInTransition(0))
        {
             ani.SetBool("attack1", false);

             //受击，完成静止
            if(isgethit)
            {
                ani.SetBool("gethit",true);
            }

            else if(stateInfo.normalizedTime >= 1.0)
            {
                Debug.Log("attack1 finish");
                 ani.SetBool("idle",true);
            }
        }
        // 如果处于攻击2
        if (stateInfo.fullPathHash == Animator.StringToHash("Base Layer.Attack2") && !ani.IsInTransition(0))
        {
             ani.SetBool("attack2", false);
             //受击，完成静止
            if(isgethit)
            {
                ani.SetBool("gethit",true);
            }
            else if(stateInfo.normalizedTime >= 1.0)
            {
                 ani.SetBool("idle",true);
            }
        }

    }
  
}
