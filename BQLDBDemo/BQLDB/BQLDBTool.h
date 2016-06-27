//
//  BQLDBTool.h
//  BQLDB
//
//  Created by 毕青林 on 16/5/31.
//  Copyright © 2016年 毕青林. All rights reserved.
//

#import <Foundation/Foundation.h>
@class BQLDBModel;

/***************************************************************
            使用本工具之前注意:本工具提供有2个使用场景
 
 场景1:存储应用中例如用户基本信息(用户id、用户昵称、用户性别等等),在你创建表开始,后续对表进行更新、查询都是针对表中唯一一条数据的,也就是这种场景是表中只存在一条记录.
 场景2:类似备忘录应用,在你创建表开始,后续对表进行更新、查询都需要用一个唯一标识符区分你要更新的记录(例如我要更新昨天写的备忘录而不是今天刚写的),也就是表中存在多条数据,需要有个标识符区分.
 
 🔽***************************** warning *****************************🔽
 
 特别注意:工具在创建数据库的时候字段是根据模型而定的,比如模型中只有name、age、sex,在创建表时候就创建对应name、age、sex三个key,在你使用:
 
    + (instancetype )modelWithDictionary:(NSDictionary*)dictionary;
 
    的时候,参数dictionary中的键值对可以少于模型定义的key(如:@{@"name":@"jack",@"age":@"18"}),
 
    但是不能多于模型定义的key(如@{@"name":@"jack",@"age":@"18",@"adress":@"上海市"}),
 
    key也不能与模型有差异(如@{@"name":@"jjj",@"agg":@"18"}),
    但不区分大小写(如@{@"Name":@"jack",@"Age":@"18"}是OK的)
 
    (举例场景:你的模型key应该是根据后台返回的json数据而定,因此key也应该是唯一对应的)
    (规范有点多,但是只要遵守了,本工具可以帮你省去很多麻烦你,你只需要规定好模型数据其他就可以不用管了)
 
 🔼***************************** warning *****************************🔼
 
 最后:
    本工具是小弟在FMDB基础上所写,当然要感谢FMDB作者啦！不管好与坏勿喷,也请尊重我的劳动~
 
    有任何问题请联系我QQ931237936
 
    PS:
        小弟整理的另一个使用官方SDK实现三方登陆分享工具:BQLAuthEngineDemo
        github地址:https://github.com/biqinglin/BQLAuthEngineDemo
 
    感谢大家支持!~~~
 
 ***************************************************************/

@interface BQLDBTool : NSObject

/**
 *  实例化单例
 */
+ (instancetype)instantiateTool;

/**
 *  打开或者创建数据库
 *
 *  @param dbName 数据库名
 *  @param model 数据模型
 */
- (void)openDBWith:(NSString *)dbName Model:(BQLDBModel *)model;

/**
 *  插入模型数据
 *  本方法会检测模型是否变更(新增字段)
 *
 *  @param dbName 数据库名
 *  @param model  数据模型
 *
 *  @return 插入成功或者失败(YES or NO)
 */
- (BOOL)insertDataWith:(NSString *)dbName Model:(BQLDBModel *)model;

/**
 *  批量插入模型数据(可选择是否用事务处理)
 *  本方法会检测模型是否变更(新增字段)
 *
 *  @param dbName         数据库名
 *  @param dataArray      数据模型数组(注意数组元素是模型不是其他,大批量插入数据建议使用useTransaction加快速度)
 *  @param useTransaction 是否使用事务处理
 *
 *  @return 插入成功或者失败(YES or NO)
 */
- (BOOL)insertDataWithBatch:(NSString *)dbName
                  DataArray:(NSArray *)dataArray
             UseTransaction:(BOOL )useTransaction;

/**
 *  批量插入模型数据(可选择是否用事务处理)
 *  本方法会检测模型是否变更(新增字段)
 *
 *  @param dbName         数据库名
 *  @param dataArray      数据模型数组(注意数组元素是模型不是其他,大批量插入数据建议使用useTransaction加快速度)
 *  @param useTransaction 是否使用事务处理
 *  @param progress       进度显示:0.0->1.0
 *
 *  @return 插入成功或者失败(YES or NO)
 */
- (BOOL)insertDataWithBatch:(NSString *)dbName
                  DataArray:(NSArray *)dataArray
             UseTransaction:(BOOL )useTransaction
                   Progress:(void(^)(double progress))progress;



/*************************** ***************************
 
 数据操作注意:默认建表时有一个主键id,可以作为标示符,
 不过我建议你自己再创建一个伪主键作为标示符 如学生id、身份证id这种 
 因为删除了id,到时候你查表的时候会发现id不是连续的,不利于你统计数据
 当然你就用id为主键执行操作也是可以的~~~
 
 *************************** ***************************/

/**
 *  删除模型数据(删除整个表)
 *
 *  @param dbName 数据库名
 *
 *  @return 删除成功或者失败(YES or NO)
 */
- (BOOL)deleteDataWith:(NSString *)dbName;

/**
 *  删除模型数据(删除表中某条数据)
 *
 *  @param dbName           数据库名
 *  @param identifier       标示符(唯一区分表中多条记录(不传默认使用建表主键:id))
 *  @param identifierValue  标示符值
 *  (例如我要删除备忘录表中MemoId = 5的那条备忘录记录,这里MemoId代表identifiers,5代表identifierValue)
 *  (例如我要删除学生表表中StuId = 1002的那条学生记录(他被开除了~~~),这里StuId代表identifiers,1002代表identifierValue)
 *
 *  @return 删除成功或者失败(YES or NO)
 */
- (BOOL)deleteDataWith:(NSString *)dbName
            Identifier:(NSString *)identifier
       IdentifierValue:(NSString *)identifierValue;

/**
 *  批量删除模型数据(删除表中多条数据)
 *
 *  @param dbName           数据库名
 *  @param identifier       标示符(唯一区分表中多条记录(不传默认使用建表主键:id))
 *  @param identifierValues 标示符值数组
 *  (例如我要删除备忘录表中MemoId = 5、MemoId = 6的那两条备忘录记录,这里MemoId代表identifiers,5、6代表identifierValues数组)
 *  (例如我要删除学生表表中StuId = 1002、StuId = 1006、StuId = 1010的那三条学生记录(他们搞基被开除了~~~),这里StuId代表identifiers,1002、1006、1010代表identifierValues数组)
 *
 *  @return 删除成功或者失败(YES or NO)
 */
- (BOOL)delectDataWithBatch:(NSString *)dbName
                 Identifier:(NSString *)identifier
           IdentifierValues:(NSArray *)identifierValues;

/**
 *  修改模型数据(单条记录:修改模型中差异数据,注意:该方法只适用于单条数据,当用在多条数据情况下会把所有数据都变成一样,因此使用者应该特别注意,修改某条数据见下面带标示符的方法)
 *
 *  @param dbName 数据库名
 *  @param model  数据模型
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWith:(NSString *)dbName Model:(BQLDBModel *)model;

/**
 *  修改模型数据(多条记录:根据标示符找到该条记录修改模型中差异数据)
 *  (例如我要修改学生表中张三(唯一标示id = 1003)的成绩(修改差异数据),这里identifier表示张三的学号(id),1003即identifierValue)
 *
 *  @param dbName           数据库名
 *  @param model            数据模型
 *  @param identifier       标示符(不传默认使用建表主键:id)
 *  @param identifierValue  标示符值
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWith:(NSString *)dbName
                 Model:(BQLDBModel *)model
            Identifier:(NSString *)identifier
       IdentifierValue:(NSString *)identifierValue;

/**
 *  修改模型数据(单条记录:根据标示符找到记录再根据key修改其对应value,该方法使用默认建表的主键id,因此与下面一个方法区分)
 *  (单条记录使用默认建表主键id即等于1)
 *
 *  @param dbName           数据库名
 *  @param key              所修改的key值(这里如果你传了一个错的key不会crash,请自行查看错误打印,尽量避免传入错误key)
 *  @param value            所修改的value值
 *  @param identifier       标示符(不传默认使用建表主键:id)
 *  @param identifierValue  标示符值
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWith:(NSString *)dbName
                   Key:(NSString *)key
                 Value:(id )value;

/**
 *  修改模型数据(单条记录:根据标示符找到记录再根据key修改其对应value,该方法使用自定义主键(stuid、bookid...),因此与上面一个方法区分)
 *  (例如我要修改学生表中张三(唯一标示stuid = 1003)的数学成绩,这里identifier表示张三的学号(stuid),1003即identifierValue)
 *
 *  @param dbName           数据库名
 *  @param key              所修改的key值(这里如果你传了一个错的key不会crash,请自行查看错误打印,尽量避免传入错误key)
 *  @param value            所修改的value值
 *  @param identifier       标示符(不传默认使用建表主键:id)
 *  @param identifierValue  标示符值
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWith:(NSString *)dbName
                   Key:(NSString *)key
                 Value:(id )value
            Identifier:(NSString *)identifier
       IdentifierValue:(NSString *)identifierValue;

/**
 *  批量修改模型数据(多条记录:根据标示符找到记录再根据key修改其对应value)
 *  (若你把数组内key与value顺序搞错了就麻烦啦~~~,所以一定要注意哦!下面提供了字典方法,怕弄错顺序就用下面的方法!~)
 *  (例如我要修改学生表中张三(唯一标示id = 1003)的数学成绩和语文成绩分别为90分、99分,这里identifier表示张三的学号(id),1003即identifierValue,keys表示@[math,chinese],values表示@[@90,@99])
 *
 *  @param dbName           数据库名
 *  @param keys             所修改的key值数组
 *  @param values           所修改的value值数组
 *  @param identifier       标示符(不传默认使用建表主键:id)
 *  @param identifierValue  标示符值
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWithBatch:(NSString *)dbName
                        Key:(NSArray *)keys
                      Value:(NSArray *)values
                 Identifier:(NSString *)identifier
            IdentifierValue:(NSString *)identifierValue;

/**
 *  批量修改模型数据(多条记录:根据标示符找到记录再根据key修改其对应value)
 *  (该方法与上面的一样,只不过用字典替换数组,上面数组方法若你把数组内key与value顺序搞错了就麻烦啦~~~)
 *
 *  @param dbName          数据库名
 *  @param keyValue        变更数据字典
 *  @param identifier      标示符(不传默认使用建表主键:id)
 *  @param identifierValue 标示符值
 *
 *  @return 修改成功或者失败(YES or NO)
 */
- (BOOL)modifyDataWithBatch:(NSString *)dbName
                   KeyValue:(NSDictionary *)keyValue
                 Identifier:(NSString *)identifier
            IdentifierValue:(NSString *)identifierValue;

/**
 *  查询数据
 *  (再次提醒注意:这里返回的数组内元素是字典类型,你可用对应模型进行转换)
 *  (建议一个做法:数据库名写在模型头文件,这个方法传的是数据库名,如果你有很多模型的话这样就可以快速知道你该用哪个模型类接收数据并转换为模型了)
    一般开发中用途:
                1:例如存储用户基本信息，这里基本是一张表一条记录，此时返回的数组中可以无数据,也可以有一条数据
                2:类似备忘录需求,这里是一张表但有多条记录，此时返回的数组中可以无数据,可以有一条数据,也可以有多条数据)
 *
 *  @param dbName 数据库名
 *
 *  @return 所查询的数据
 */
- (NSArray *)queryDataWith:(NSString *)dbName;


@end







