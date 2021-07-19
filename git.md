## .gitignore 파일 업데이트 후 적용 시
~~~
git rm -r --cached .
git add .
git commit -m "update applied"
~~~
</br>

## 로컬의 프로젝트를 깃허브로 올릴 때
1. 깃 허브에서 레포지토리 생성(이 때 readme, .gitignore 는 생성하지 않음)
2. 로컬의 프로젝트로 이동 후 아래와 같이 명령어 실행
~~~
git init
git remote add origin "레포지토리 주소"
git add .
git commit -m "commit messages"
git push -u origin master
~~~
