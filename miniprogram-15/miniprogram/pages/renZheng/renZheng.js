Page({
  data:{
    id:"",
    key:""
  },
  input1:function(e){          //获得输入的devicesid
    var id = e.detail.value
    this.setData({
      id:id
    })
  },
  input2: function (e) {          //获得输入的qpi_key
    var key = e.detail.value
    this.setData({
      key:key
    })
  },
  bind:function(){              //点击上传这两个重要参数到云数据库
    var id=this.data.id;
    var key=this.data.key;
    var db=wx.cloud.database()
    db.collection("nickNameCollection").add({
      data:{
        devicesId:id,
        api_key:key
      }
    })
    console.log("绑定ok")
  }
})