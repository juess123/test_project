import cv2
import numpy as np
import re
images_file='image.jpg'
# 配置参数
labels={0:'Rectang',1:'Dimensions',2:'100',3:'40'}
# labels= {0:"0",1:"1",2:"2",3:"3",4:"4",5:"5",6:"6",7:"7",8:"8",9:"9"},
#"labels":{0:'Line',1:'DoubleArrow',2:'SingleArrow',3:'Round1',4:'Gear',5:'Rounds1',6:'Roughness',7:'Rectangle',8:'RoundAngle',9:'Nut',10:'Table',11:'Round2',12:'Line2',13:'Number1',14:'NUmber2',15:'NUmber3',16:'Rounds2',17:'Screw',18:'ArcRectangle',19:'Oval'},
CONFIG = {
    "model_file": "./best.onnx",

   
    "model_input_size": (640, 640),  # 模型输入尺寸 (H, W)
    "nms_threshold": 0.45,
    "confidence_threshold": 0.25,
    "output_file": "./output.jpg"
}
class ObjectDetector:
    def __init__(self, config):
        self.labels = labels
        self.model_file = config["model_file"]
        self.input_size = config["model_input_size"]
        self.nms_threshold = config["nms_threshold"]
        self.conf_threshold = config["confidence_threshold"]
        self.net = cv2.dnn.readNetFromONNX(self.model_file)

    def preprocess(self, image):
        """预处理图像"""
        blob = cv2.dnn.blobFromImage(image, scalefactor=1/255.0, size=self.input_size, swapRB=True)
        return blob

    def post_process(self, outputs, original_shape):
        """后处理推理结果"""
        img_h, img_w = original_shape[:2]
        conf = outputs[:, 4]  # 置信度
        boxes = outputs[:, :4]
        p_cls = outputs[:, 5:]
        cls_id = np.argmax(p_cls, axis=1)

        # 转换边界框到原图尺寸
        c_x, c_y, w, h = boxes[:, 0], boxes[:, 1], boxes[:, 2], boxes[:, 3]
        c_x, c_y = c_x * img_w / self.input_size[1], c_y * img_h / self.input_size[0]
        w, h = w * img_w / self.input_size[1], h * img_h / self.input_size[0]

        x1, y1 = c_x - w / 2, c_y - h / 2
        x2, y2 = c_x + w / 2, c_y + h / 2
        areas = np.vstack((x1, y1, x2, y2)).T

        # NMS 去重
        indices = cv2.dnn.NMSBoxes(areas.tolist(), conf.tolist(), self.conf_threshold, self.nms_threshold)

        if len(indices) > 0:
            indices = indices.flatten()
            return areas[indices], conf[indices], cls_id[indices]
        return [], [], []

    def infer(self, image):
        """执行推理"""
        blob = self.preprocess(image)
        self.net.setInput(blob)
        outputs = self.net.forward()[0]
        return outputs

    def draw_results(self, image, boxes, scores, ids):
        """绘制结果到图像"""
        for box, score, id_ in zip(boxes, scores, ids):
            x1, y1, x2, y2 = map(int, box)
            label = f"{self.labels[id_]} {score:.2f}"
            color = (0, 0, 255)  # 红色

            # 绘制边界框
            cv2.rectangle(image, (x1, y1), (x2, y2), color, 2)

            # 绘制标签
            font_scale = 0.5
            font_thickness = 1
            label_size = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, font_scale, font_thickness)[0]
            cv2.rectangle(image, (x1, y1 - label_size[1] - 5), (x1 + label_size[0], y1), color, -1)
            cv2.putText(image, label, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, font_scale, (255, 255, 255), font_thickness)

        return image
    def display_results(self, boxes, scores, ids):
        """展示所有识别到的元素及其得分和坐标框"""
        print("\n识别结果:")
        if len(boxes) == 0:
            print("未识别到任何目标。")
        else:
            for i, (box, score, id_) in enumerate(zip(boxes, scores, ids), start=1):
                x1, y1, x2, y2 = map(int, box)
                print(f"目标 {i}: 类别={self.labels[id_]}, 置信度={score:.2f}, 坐标=({x1}, {y1}, {x2}, {y2})")
       
    def write_to_file(self,numbers, filename="rectangle.txt"):
        try:
            with open(filename, "w") as file:
                for number in numbers:
                    file.write(f"{number}\n")
            print(f"数字已成功写入文件: {filename}")
        except Exception as e:
            print(f"写入文件时出错: {e}")

    def extract_integer(self,label):
        match = re.search(r'\d+', label)  # 使用正则表达式查找字符串中的数字部分
        if match:
            return int(match.group())  # 将匹配的数字部分转换为整数返回
        return None  # 如果没有找到数字，返回 None
    def input_num(self, ids):
        numbers = []
        for id_ in ids:
            label = self.labels[id_]
            extracted_number =self.extract_integer(label)
            if extracted_number is not None:
                numbers.append(extracted_number)
        sorted_numbers = sorted(numbers, reverse=True)
        self.write_to_file(sorted_numbers)


class Datahandle:
    def __init__(self,boxes,ids):
            self.boxes=boxes
            self.ids=ids
    def label_sort(self,data_type):
        result = []
        for sublist in data_type:
            if not sublist[0].isdigit():  # 判断第一个元素是否是字符
                return True
            return False
    def get_data(self):
        if len(self.boxes) == 0:
            print("未识别到任何目标。")
        else:
            label_list=[]
            num_list=[]
            for i, (box,id_) in enumerate(zip(self.boxes,self.ids), start=1):
                x1, y1, x2, y2 = map(int, box)
                if self.label_sort(labels[id_]) == 0:
                    label_list.append([labels[id_],(x1+x2)/2,(y1+y2)/2])
                else:
                    num_list.append([labels[id_],(x1+x1)/2,y1,(x1+x2)/2,y2,x1,(y1+y2)/2,x2,(y1+y2)/2])
        self.num_list=num_list
        self.label_list=label_list
        return label_list,num_list
    # def sort_data(num_list,label_list):
    #     label_x=label_list[1]
    #     label_y=label_list[2]
    #     num_x=num_list[1]
    #     num_y=num_list[2]
    #     for x,y 
    # def match_algorithm(self):
    #     label_list_len=len(self.label_list)
    #     i=1
    #     while(i<label_list_len):
    #         self.label_list[]


def main(file_image, config):
    detector = ObjectDetector(config)
    image = cv2.imread(file_image)
    original_shape = image.shape
    outputs = detector.infer(image)
    boxes, scores, ids = detector.post_process(outputs, original_shape)


    Data=Datahandle(boxes,ids)
    label_list,num_list =Data.get_data()
    for label in enumerate(label_list):
        print(label)
    for label in enumerate(num_list):
        print(label)

    
    cv2.imshow("test",image)
    cv2.waitKey()
    cv2.destroyAllWindows()

# 执行
main(images_file, CONFIG)
# image=cv2.imread("./image.jpg")
# cv2.rectangle(image, (183, 231), (200, 257), (0,0,255), 1)
# cv2.imshow("Rectangle", image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()