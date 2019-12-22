//app.js
App({
  onLaunch: function () {
    if (!wx.cloud) {
      console.error('请使用 2.2.3 或以上的基础库以使用云能力')
    } else {
      wx.cloud.init({
        env:'cloud-bd83g',       //环境id
        traceUser: true,
      })
    }
    const db = wx.cloud.database({
      env: 'cloud-bd83g'
    })
    db.collection('nickNameCollection').where({      //从所设的集合中获取到用户标识openid，便于识别
                                                     //登录的用户，从而进行大棚设备绑定
      _openid: '_openid',
    })
      .get({
        success: res => {
          console.log(res.data[res.data.length - 1]);            //更改全局变量，获取对应用户的设备id和api_key,实现绑定
          this.globalData.devicesId=res.data[res.data.length-1].devicesId,
            this.globalData.api_key = res.data[res.data.length - 1].api_key          //已成功
        }
      })
    this.globalData = {
      devicesId: '',
      api_key: ''
    }
  }
})
