import torch
import detectron2
from detectron2.utils.logger import setup_logger
setup_logger()

from detectron2 import model_zoo
from detectron2.engine import DefaultPredictor
from detectron2.config import get_cfg

cfg = get_cfg()

#Model1: mask_rcnn_R_101_FPN_3x
cfg.merge_from_file(model_zoo.get_config_file("COCO-InstanceSegmentation/mask_rcnn_R_101_FPN_3x.yaml"))
cfg.MODEL.ROI_HEADS.SCORE_THRESH_TEST = 0.5
cfg.MODEL.WEIGHTS = model_zoo.get_checkpoint_url("COCO-InstanceSegmentation/mask_rcnn_R_101_FPN_3x.yaml")
predictorRCNN101 = DefaultPredictor(cfg)

#Model2: retinanet_R_101_FPN_1x
cfg.merge_from_file(model_zoo.get_config_file("COCO-Detection/retinanet_R_101_FPN_3x.yaml"))
cfg.MODEL.ROI_HEADS.SCORE_THRESH_TEST = 0.5
cfg.MODEL.WEIGHTS = model_zoo.get_checkpoint_url("COCO-Detection/retinanet_R_101_FPN_3x.yaml")
predictorRet101 = DefaultPredictor(cfg)

# Valdation Dataset
from detectron2.data.datasets import register_coco_instances
register_coco_instances("coco_val_2017", {}, "./dataset/annotations/instances_val2017.json", "./dataset/val2017")


# Evaluation
from detectron2.evaluation import inference_on_dataset, COCOEvaluator
from detectron2.data import build_detection_test_loader

evaluator = COCOEvaluator("coco_val_2017", output_dir="./outputRetina")
val_loader = build_detection_test_loader(cfg, "coco_val_2017")
inference_on_dataset(predictorRet101.model, val_loader, evaluator)

evaluator = COCOEvaluator("coco_val_2017", output_dir="./outputRCNN")
val_loader = build_detection_test_loader(cfg, "coco_val_2017")
inference_on_dataset(predictorRCNN101.model, val_loader, evaluator)