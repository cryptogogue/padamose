FROM padamose-build-base

WORKDIR /app
COPY . /app

EXPOSE 22
CMD ["/bin/bash"]
