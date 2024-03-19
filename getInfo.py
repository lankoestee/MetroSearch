import requests
import json
import random
import csv
import pandas as pd

USER_AGENTS = [
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; AcooBrowser; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Acoo Browser; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506)",
    "Mozilla/4.0 (compatible; MSIE 7.0; AOL 9.5; AOLBuild 4337.35; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 2.0.50727; Media Center PC 6.0)",
    "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 1.0.3705; .NET CLR 1.1.4322)",
    "Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.2; .NET CLR 1.1.4322; .NET CLR 2.0.50727; InfoPath.2; .NET CLR 3.0.04506.30)",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN) AppleWebKit/523.15 (KHTML, like Gecko, Safari/419.3) Arora/0.3 (Change: 287 c9dfb30)",
    "Mozilla/5.0 (X11; U; Linux; en-US) AppleWebKit/527+ (KHTML, like Gecko, Safari/419.3) Arora/0.6",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.2pre) Gecko/20070215 K-Ninja/2.1.1",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9) Gecko/20080705 Firefox/3.0 Kapiko/3.0",
    "Mozilla/5.0 (X11; Linux i686; U;) Gecko/20070322 Kazehakase/0.4.5",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko Fedora/1.9.0.8-1.fc10 Kazehakase/0.5.6",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/535.20 (KHTML, like Gecko) Chrome/19.0.1036.7 Safari/535.20",
    "Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; fr) Presto/2.9.168 Version/11.52"]

loopLine=[['beijing','2号线'],['beijing','10号线'],['shanghai','4号线'],['zhengzhou','5号线'],['chengdu','7号线'],['chongqing','环线']]

dishini=False
xingqiao=False
yuanbozhongxin=False
xinzhuang=False
guihuaxilu=False
hangzhonglu=False
daxuechengnan=False
weijianian=False
luomazhou=False
xiaoshanguojijichang=False

def getHtml(url):
    user_agent = random.choice(USER_AGENTS)
    headers = {
        "Host":"map.amap.com",
        'User-Agent': user_agent
    }
    try:
        response = requests.get(url, headers=headers)
        #print(response.url)
        text = response.content
        return text
    except:
        pass
        #print("爬取失败!")

def parse_page(text):
    lines_list = json.loads(text).get('l')
    #print(lines_list)
    # 地铁线路信息表
    lineInfo_list = []
    for line in lines_list:
        #每条线的信息集合
        lineInfo = {}
        lineInfo['ln'] = line.get('ln')
        #print(lineInfo['ln'])

        #线路站点列表
        station_list = []
        st_list = line.get('st')
        nowSize=0
        for st in st_list:
            station_dict = {}
            station_dict['name'] = st.get('n')
            coord = st.get('sl')
            station_dict['lat'] = coord.split(',')[0]
            station_dict['lon'] = coord.split(',')[-1]
            #print("站名称:", station_dict['name'])
            #print("经度：", station_dict['lat'])
            #print("纬度：", station_dict['lon'])
            if nowSize==0 or station_list[nowSize-1]['name']!=station_dict['name']:
                station_list.append(station_dict)
                nowSize+=1
            #pass
        #print('-----------------------------------')
        lineInfo['st'] = station_list
        lineInfo['kn'] = line.get('kn')
        lineInfo['ls'] = line.get('ls')
        lineInfo['cl'] = line.get('cl')
        lineInfo_list.append(lineInfo)
    #返回各线路信息列表
    return lineInfo_list


def save_file(filename,lineInfo,cityName):
    #print("开始写入文件......")
    with open(filename, 'w', encoding='utf-8',newline='') as f:
        writer=csv.writer(f)
        writer.writerow(['line','lineName','color','name','lat','lon','loop'])
        for i in range(len(lineInfo)):
            for st in lineInfo[i]['st']:
                lineName=lineInfo[i]['ln']

                if cityName=='shanghai' and lineInfo[i]['ln']=='11号线':
                    global dishini
                    if dishini is False:
                        lineName='11号线（嘉定北-迪士尼）'
                    else:
                        lineName='11号线（花桥-迪士尼）'
                    if st['name']=='迪士尼':
                        dishini=True

                if cityName=='shanghai' and lineInfo[i]['ln']=='5号线':
                    global xinzhuang
                    if xinzhuang is False:
                        lineName='5号线（闵行开发区-莘庄）'
                    else:
                        lineName='5号线（奉贤新城-莘庄）'
                    if st['name']=='莘庄':
                        xinzhuang=True

                if cityName=='shanghai' and lineInfo[i]['ln']=='10号线':
                    global hangzhonglu
                    if hangzhonglu is False:
                        lineName='10号线（基隆路-航中路）'
                    else:
                        lineName='10号线（基隆路-虹桥火车站）'
                    if st['name']=='航中路':
                        hangzhonglu=True

                if cityName=='hangzhou' and lineInfo[i]['ln']=='3号线':
                    global xingqiao
                    if xingqiao is False:
                        lineName='3号线（吴前新村-星桥）'
                    else:
                        lineName='3号线（石马-星桥）'
                    if st['name']=='星桥':
                        xingqiao=True
                
                if cityName=='hangzhou' and lineInfo[i]['ln']=='6号线':
                    global guihuaxilu
                    if guihuaxilu is False:
                        lineName='6号线（枸桔弄-桂花西路）'
                    else:
                        lineName='6号线（枸桔弄-双浦）'
                    if st['name']=='桂花西路':
                        guihuaxilu=True

                if cityName=='chongqing' and lineInfo[i]['ln']=='5号线':
                    global yuanbozhongxin
                    if yuanbozhongxin is False:
                        lineName='3号线北段'
                    else:
                        lineName='3号线南段'
                    if st['name']=='园博中心':
                        yuanbozhongxin=True

                if cityName=='kunming' and lineInfo[i]['ln']=='1号线':
                    global daxuechengnan
                    if daxuechengnan is False:
                        lineName='1号线（环城南路-大学城南）'
                    else:
                        lineName='1号线（环城南路-昆明南火车站）'
                    if st['name']=='大学城南':
                        daxuechengnan=True

                if cityName=='chengdu' and lineInfo[i]['ln']=='1号线':
                    global weijianian
                    if weijianian is False:
                        lineName='1号线（五根松-韦家碾）'
                    else:
                        lineName='1号线（科学城-韦家碾）'
                    if st['name']=='韦家碾':
                        weijianian=True
                
                if cityName=='xianggang' and lineInfo[i]['ln']=='东铁线':
                    global luomazhou
                    if luomazhou is False:
                        lineName='东铁线（金钟-落马洲）'
                    else:
                        lineName='东铁线（金钟-罗湖）'
                    if st['name']=='落马洲':
                        luomazhou=True 

                if cityName=='hangzhou' and lineInfo[i]['ln']=='1号线':
                    global xiaoshanguojijichang
                    if st['name']=='萧山国际机场':
                        xiaoshanguojijichang=True
                    elif xiaoshanguojijichang is True:
                        continue
                
                # if cityName=='hangzhou' and lineInfo[i]['ln']=='6号线' and st['name']=='奥体中心':
                #     st['name']='奥体中心（杭州）'
                
                # if cityName=='hangzhou' and lineInfo[i]['ln']=='7号线' and st['name']=='奥体中心':
                #     st['name']='奥体中心（杭州）'

                # if cityName=='hangzhou' and lineInfo[i]['ln']=='绍兴1号线' and st['name']=='奥体中心':
                #     st['name']='奥体中心（绍兴）'

                if [cityName,lineInfo[i]['ln']] in loopLine:
                    writer.writerow([lineName,lineInfo[i]['kn'],''.join(['#',lineInfo[i]['cl']]),st['name'],st['lat'],st['lon'],str(1)])
                else:
                    writer.writerow([lineName,lineInfo[i]['kn'],''.join(['#',lineInfo[i]['cl']]),st['name'],st['lat'],st['lon'],str(0)])
    #print("写入文件完成！")

Index=pd.read_csv('./city/cityIndex.csv')

for i in range(len(Index['city'])):
    if Index['url'][i]!='false':
        cityName=Index['city'][i]
        url=Index['url'][i]
        fileName=''.join(['./city/',cityName,'.csv'])
        text=getHtml(url)
        lineinfo=parse_page(text)
        #print(lineinfo)
        save_file(fileName,lineinfo,cityName)