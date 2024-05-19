# ReadMe
## 简介
一个读取罗技G29方向盘的Qt上位机。
## Note
```
/*
    "刹车" << m_g29->m_data->lRz
    "方向盘" << m_g29->m_data->lX
    "油门" << m_g29->m_data->lY
    "升档" << m_g29->m_data->rgbButtons[4]
    "降档" << m_g29->m_data->rgbButtons[5]
*/
```
## ToDoList
- [ ] 实现换挡拨片检测
- [ ] 实现数据显示
- [ ] 实现数据上传MQTT服务器