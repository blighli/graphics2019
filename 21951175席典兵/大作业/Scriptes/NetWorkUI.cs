using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class NetWorkUI : MonoBehaviour {
    //开始服务器连接
    //该方法等同于LAN Host(H)
    public void StartHost()
    {
        //通过NetworkManager的单列，调用其中的StartHost()方法
        NetworkManager.singleton.StartHost();
       
    }
        
    //该方法等同于Start Client(C)
    public void StartClient()
    {
        //GameObject.Find("IP").GetComponent<Text>();无法获取到该text，因为组件名为InputField而非text
        NetworkManager.singleton.networkAddress = GameObject.Find("IP").GetComponent<InputField>().text;
        //开始客户端连接
        NetworkManager.singleton.StartClient();
    }
    //该方法等同于StopHost
    public void StopHost()
    {
        //退出连接
        NetworkManager.singleton.StopHost();
    }

    //对离线场景的按钮添加监听事件
    public void OfflineSet()
    {
        //对Host按钮设置按钮监听事件
        GameObject.Find("Host").GetComponent<Button>().onClick.AddListener(StartHost);
        //对Client按钮设置监听事件
        GameObject.Find("Client").GetComponent<Button>().onClick.AddListener(StartClient);
    }
    //对在线场景的按钮添加监听事件
    public void OnlineSet()
    {
        GameObject.Find("ReturnBtn").GetComponent<Button>().onClick.AddListener(StopHost);
    }

    ///*C#委托*/
    //// called first
    //void OnEnable()
    //{
    //    Debug.Log("OnEnable called");
    //    SceneManager.sceneLoaded += OnSceneLoaded;//委托的多播
    //}

    //// called second
    //void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    //{
    //    Debug.Log("OnSceneLoaded: " + scene.name);
    //    ////没有激活局域网端的UI，要把此段注释掉
    //    //switch (scene.buildIndex)
    //    //{
    //    //    case 0:
    //    //        OfflineSet();
    //    //        break;
    //    //    case 1:
    //    //        OnlineSet();
    //    //        break;
    //    //    default:
    //    //        break;
    //    //}
    //}

    //// called when the game is terminated
    //void OnDisable()
    //{
    //    Debug.Log("OnDisable");
    //    SceneManager.sceneLoaded -= OnSceneLoaded;//委托的多播
    //}
    ///****************/
}
