import json
#Ensemble

#IOU

def iou(boxA, boxB):
	# determine the (x, y)-coordinates of the intersection rectangle
	xA = max(boxA[0], boxB[0])
	yA = max(boxA[1], boxB[1])
	xB = min(boxA[2], boxB[2])
	yB = min(boxA[3], boxB[3])
	# compute the area of intersection rectangle
	interArea = max(0, xB - xA + 1) * max(0, yB - yA + 1)
	# compute the area of both the prediction and ground-truth
	# rectangles
	boxAArea = (boxA[2] - boxA[0] + 1) * (boxA[3] - boxA[1] + 1)
	boxBArea = (boxB[2] - boxB[0] + 1) * (boxB[3] - boxB[1] + 1)
	# compute the intersection over union by taking the intersection
	# area and dividing it by the sum of prediction + ground-truth
	# areas - the interesection area
	iou = interArea / float(boxAArea + boxBArea - interArea)
	# return the intersection over union value
	return iou

#Load Annotations
with open('./dataset/annotations/instances_val2017.json') as f_annot:
    val_data = json.load(f_annot)
annotations = val_data["annotations"]

# Load Annotations Retina101 predictions
with open('./outputRetina/coco_instances_results.json') as f_pred_Ret:
    predRet = json.load(f_pred_Ret)
print("Retina preds: ", len(predRet))

# Load Annotations RCNN predictions
with open('./outputFRCNN/coco_instances_results.json') as f_pred_FRCNN:
    predFRCNN = json.load(f_pred_FRCNN)
print("RCNN preds: ", len(predFRCNN))

area_range_s_m = [0 ** 2,  96 ** 2] # small and medium objects based on cocoapi

#Remove FRCNN101 model's small objects results
for idx, pred in enumerate(predRet):
    bbox = pred['bbox']
    bbox_area = bbox[2] * bbox[3]
    if bbox_area > area_range_s_m[1] and predRet[idx]['score'] > 0.6:
        for pred_rcnn in predFRCNN:
            rcnn_box = pred_rcnn['bbox']
            if pred_rcnn["image_id"] == pred["image_id"] and iou(bbox, rcnn_box) > 0.7:
                 predRet[idx]['bbox'] = [(x + y)/2.0 for x, y in zip(predRet[idx]['bbox'], rcnn_box)]
                 predRet[idx]['score'] = pred_rcnn['score']
#save ensemble results
with open('ensemble_results.json', 'w') as f:
    json.dump(predRet, f)

# Evaluation COCO api
from pycocotools.coco import COCO
from pycocotools.cocoeval import COCOeval

annType = ['segm','bbox','keypoints']
annType = annType[1]
#initialize COCO ground truth api
annFile = "./dataset/annotations/instances_val2017.json"
cocoGt=COCO(annFile)

print("********* Ensemble results ********* ")
resFile = "./ensemble_results.json"
cocoDt=cocoGt.loadRes(resFile)
cocoEval = COCOeval(cocoGt,cocoDt,annType)
cocoEval.evaluate()
cocoEval.accumulate()
cocoEval.summarize()

print("********* RCNN Results *********")
resFile = "./outputFRCNN/coco_instances_results.json"
cocoDt=cocoGt.loadRes(resFile)
cocoEval = COCOeval(cocoGt,cocoDt,annType)
cocoEval.evaluate()
cocoEval.accumulate()
cocoEval.summarize()

print("********* Retina Results *********")
resFile = "./outputRetina/coco_instances_results.json"
cocoDt=cocoGt.loadRes(resFile)
cocoEval = COCOeval(cocoGt,cocoDt,annType)
cocoEval.evaluate()
cocoEval.accumulate()
cocoEval.summarize()
