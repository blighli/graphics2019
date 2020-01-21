using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.Match;
using UnityEngine.UI;

public class MatchMaker : MonoBehaviour {

    //定义结点，方便后序删除指定房间
    public class Node
    {
        public string name;
        public GameObject go;
        public Node(string s, GameObject g) 
        {
            this.name = s;
            this.go = g;
        }
    };
    static MatchMaker _instance;

    public static MatchMaker Instance
    {
        get
        {
            return _instance;
        }
    }
    private void Awake()
    {
        if (Instance == null)
        {
            _instance = this;
        }
    }
    //由于之后会经常使用NetworkManager的单列，所以在此处创建
    NetworkManager manager;
    //存储房间信息
    public List<Node> roomList = new List<Node>();
    //房间名
    public string roomName;
    //刷新按钮
    [SerializeField]
    GameObject btn;
    //显示Content的内容
    [SerializeField]
    Transform parent;

    

    void Start()
    {
        manager = NetworkManager.singleton;
        //如果还没有匹配，则开始匹配
        if (manager.matchMaker == null)
            manager.StartMatchMaker();
    }
    //设置房间名
    public void SetRoomName(string name)
    {
        //视频里的人写错了
        //她直接写成了roomName=name,实现错误
        roomName = GameObject.Find("RoomInput").GetComponent<InputField>().text;
    }
    //创建房间
    public void OnCreateRoomBtn()
    {
        //参数见Unity的API
        //使用了默认的回调函数，采用回调函数即是使用了委托
        manager.matchMaker.CreateMatch(roomName, 3, true, "", "", "", 0, 0, manager.OnMatchCreate);
        
    }
    //刷新按钮监听事件
    public void OnRefreshBtn()
    {
        manager.matchMaker.ListMatches(0, 10, "", true, 0, 0, OnMatchList);
    }
    //
    public void OnMatchList(bool success,string extendedInfo,List<MatchInfoSnapshot> matches)
    {
        //搜索不成功
        if (!success)
        {
            Debug.Log("error");
            return;
        }
        ClearList();
        foreach (var match in matches)
        {
            print("match.name是：" + match.name);
            //要生成的对象，生成的位置
            GameObject go = Instantiate(btn, parent);
            //设置房间名
            go.transform.Find("RoomName").GetComponent<Text>().text = match.name;
            //生成游戏物体的同时获取info,即match，通过调用物体的组件形式，来调用此方法
            go.GetComponent<JoinButton>().SetUp(match);
            //go.GetComponent<Text>().text = match.name;
            //加入到链表中
            Node node=new Node(match.name,go);
            roomList.Add(node);
        }
    }
    void ClearList()
    {
        int count = roomList.Count;
        for(int i=0; i< count; i++)
        {
            //删除roomList中的每个对象
            Destroy(roomList[i].go);
            //在链表中移除对象
            roomList.RemoveAt(i);            
        }
        //链表清空
        roomList.Clear();
    }
    //返回到游戏主页
    public void ReturnHome()
    {
        //重新激活游戏
        UnityEngine.SceneManagement.SceneManager.LoadScene(0);
    }
}
