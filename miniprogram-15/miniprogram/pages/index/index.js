Page({
  data:{
    nickName:"",
  },
  renZheng:function(){        //认证页面跳转
    wx.navigateTo({
      url: '/pages/renZheng/renZheng',
    })
  },
  shiCha:function(){           //视察页面跳转
    wx.navigateTo({
      url: '../../pages/shiCha/shiCha',
    })
  },
  onLoad: function (options) {          //在首页获取和显示昵称，达到亲切的欢迎效果，增加用户体验
    var that = this;
    //获取用户信息,昵称
    wx.getUserInfo({
      success: function (res) {
        console.log(res.userInfo.nickName,"登录了");  //在控制台也显示一下
        that.data.nickName = res.userInfo.nickName;
        that.setData({
          nickName: that.data.nickName
        })
      }
    })
      // 调用云函数,目的是获取_openid
      wx.cloud.callFunction({
        name: 'login',
        data: {},
        success: res => {
          console.log('[云函数] [login] user openid: ', res.result.openid)
        },
        fail: err => {
          console.error('[云函数] [login] 调用失败', err)
        }
      })
    const db = wx.cloud.database()
  }
})