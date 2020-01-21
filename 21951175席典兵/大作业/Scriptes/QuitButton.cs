using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.Match;
using UnityEngine.UI;

public class QuitButton : MonoBehaviour {

    MatchInfoSnapshot info;
    //玩家加入
    public void SetUp(MatchInfoSnapshot _info)
    {
        info = _info;
        //设置房间名称，此处可以不写这句，因为我之前在MatchMaker中已经写过设置名称了
        transform.Find("RoomName").GetComponent<Text>().text = info.name;
    }
    //断开连接并退出
    //public void OnQuitBtn()
    //{
    //    //断开连接
    //    NetworkManager.singleton.matchMaker.DropConnection(NetworkManager.singleton.matchInfo.networkId, NetworkManager.singleton.matchInfo.nodeId,
    //        0, NetworkManager.singleton.OnDropConnection);
    //    //删除此次匹配，选择默认回调函数，使用下局代码未成功
    //    //NetworkManager.singleton.matchMaker.DestroyMatch(NetworkManager.singleton.matchInfo.networkId, 0, NetworkManager.singleton.OnDestroyMatch);
    //    ////删除该房间
    //    //int index = 0;
    //    //bool flag = false;
    //    //for (int i = 0; i < MatchMaker.Instance.roomList.Count; i++)
    //    //{
    //    //    if (MatchMaker.Instance.roomList[i].name == info.name)
    //    //    {
    //    //        index = i;
    //    //        flag = true;
    //    //        break;
    //    //    }
    //    //}
    //    //if (flag)
    //    //{
    //    //    //删除roomList中的这个对象
    //    //    Destroy((MatchMaker.Instance.roomList[index].go));
    //    //    //在链表中移除对象
    //    //    MatchMaker.Instance.roomList.RemoveAt(index);
    //    //}
    //    //退出
    //    NetworkManager.singleton.StopHost();
    //}

}
