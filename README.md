# AI Detection 서비스 및 관련 논문

## AI Detection 서비스

1. **ZeroGPT**  
   [ZeroGPT 웹사이트](https://www.zerogpt.com/)  
   ZeroGPT는 GPT-3, LLaMA, Google Bard와 같은 AI 모델이 생성한 콘텐츠를 감지하는 도구입니다. 한글도 지원하지만, 학습 데이터가 주로 영어 중심이라 한글에 대한 정확도가 매우 낮습니다.

2. **GPTZero**  
   [GPTZero 웹사이트](https://gptzero.me/)  
   GPTZero는 AI 생성 텍스트를 탐지합니다. 모든 언어를 지원한다고 하나, 한글에 대한 정확도는 낮다는 피드백이 많습니다. 특히 한글 콘텐츠가 AI 생성이라고 잘못 판단하는 경우(거짓 양성)가 자주 발생합니다.

3. **OpenAI - AI Text Classifier**  
   [OpenAI AI Classifier](https://openai.com/index/new-ai-classifier-for-indicating-ai-written-text/)  
   OpenAI는 AI 텍스트 분류기의 정확도가 낮다고 경고하고 있으며, 현재는 사용 불가 상태입니다. 이 분류기는 주로 영어 텍스트에 사용되는 것이 좋으며, 9%의 거짓 양성(FP) 오류를 가질 수 있습니다. 훈련 과정에서 AI 생성 텍스트와 인간 텍스트 간의 차이를 정확하게 구별하는 것이 매우 어려운 일임을 알 수 있습니다.

---

## 논문

1. **Can AI-Generated Text be Reliably Detected?**  
   [논문 링크](https://ar5iv.labs.arxiv.org/html/2303.11156)  
   이 논문은 대형 언어 모델(LLMs)로 생성된 텍스트의 탐지가 패러프레이징(재구성)에 취약하며, 탐지 성능이 랜덤 분류기 수준으로 떨어질 수 있다는 것을 입증합니다. 특히 AI 생성 텍스트의 탐지 한계는 특정 글쓰기 스타일이나 고의적 조작까지도 가능하게 만든다고 설명합니다. 또한, 워터마킹이 적용된 모델도 공격에 취약하다는 점을 강조하고 있습니다.

2. **Safeguarding Authenticity in Text with BERT-Powered Detection of AI-Generated Content**  
   [IEEE 논문 링크](https://ieeexplore.ieee.org/document/10544590)  
   이 연구는 BERT 모델을 활용하여 AI 생성 텍스트를 식별하는 방법을 제시합니다. BERT의 문맥적 임베딩을 분석하여 AI와 인간 작성 텍스트를 구별할 수 있는 성능을 입증하며, 디지털 환경에서 인간 저작물의 진정성을 보호하는 데 중요한 기여를 하고자 합니다.
