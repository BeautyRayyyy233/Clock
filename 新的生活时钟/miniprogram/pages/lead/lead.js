Page({
  data:{

  },

  clock:e=>{
    wx.navigateTo({
      url: '../index/index'
    })
  },

  memo:e=>{
    wx.navigateTo({
      url: '../diary/diary',
    })
  }
})