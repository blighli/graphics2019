using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.Match;
using UnityEngine.UI;

public class JoinButton : MonoBehaviour {
    MatchInfoSnapshot info;
    //玩家加入
    public void SetUp(MatchInfoSnapshot _info)
    {
        info = _info;
        //设置房间名称，此处可以不写这句，因为我之前在MatchMaker中已经写过设置名称了
        transform.Find("RoomName").GetComponent<Text>().text = info.name;
    }
    //注册加入房间事件
    public void OnJointBtn()
    {
        //使用默认的回调函数
        NetworkManager.singleton.matchMaker.JoinMatch(info.networkId, "", "", "", 0, 0, NetworkManager.singleton.OnMatchJoined);
    }
}
